# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <dirent.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "parse.h"
# include "executes.h"

void run_out_redirect_command(char * command) {
	char ** commands = parse_args(command, ">");
	char ** args = parse_args(remove_spaces(commands[0]), " ");
	char * file_name = remove_spaces(commands[1]);

	int fd = open(file_name, O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0755);
	if (fd == -1) {
		fd = open(file_name, O_WRONLY|O_TRUNC);
	}

	if (fork() == 0) {
		dup2(fd, STDOUT_FILENO);
		// if execvp errors and cannot run
		if (execvp(args[0], args) == -1)
			exit(-1);
	}
	free(commands[0]);
	free(commands);
	free(args[0]);
	free(args);

	close(fd);
}

void run_in_redirect_command(char * command) {
  char ** commands = parse_args(command, "<");
	char ** args = parse_args(remove_spaces(commands[0]), " ");
	char * file_name = remove_spaces(commands[1]);

	int fd = open(file_name, O_RDONLY);

	if (fork() == 0) {
		dup2(fd, STDIN_FILENO);
		if (execvp(args[0], args) == -1)
			exit(-1);
	}
	free(commands[0]);
	free(commands);
	free(args[0]);
	free(args);

	close(fd);
}

void run_in_out_redirect_command(char * command) {
	char ** commands = parse_args(command, "<>");
	char ** args = parse_args(remove_spaces(commands[0]), " ");
	char * file_name = remove_spaces(commands[1]);
	char * file_name_2 = remove_spaces(commands[2]);

	int fd = open(file_name, O_RDONLY);
	int fd2 = open(file_name_2, O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0755);
        if (fd2 == -1) {
                fd2 = open(file_name_2, O_WRONLY|O_TRUNC);
        }
	if (fork() == 0) {
		dup2(fd, STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		if (execvp(args[0], args) == -1)
			exit(-1);
	}

	free(commands[0]);
	free(commands);
	free(args[0]);
	free(args);

	close(fd);
	close(fd2);
}

void run_pipe_command(char * command) {
	char ** commands = parse_args(command, "|");
	char ** first_command = parse_args(remove_spaces(commands[0]), " ");
	char ** second_command = parse_args(remove_spaces(commands[1]), " ");

	int mypipe[2];
	pipe(mypipe);

	if (fork() == 0) {
	  dup2(mypipe[1], STDOUT_FILENO);
		if (execvp(first_command[0], first_command) == -1)
			exit(-1);
	}

	int w;
	wait(&w);

	if (fork() == 0) {
		dup2(mypipe[0], STDIN_FILENO);
		close(mypipe[1]);
		if (execvp(second_command[0], second_command) == -1)
			exit(-1);
	}

	free(commands[0]);
	free(commands);
	free(first_command[0]);
	free(first_command);
	free(second_command[0]);
	free(second_command);

	close(mypipe[0]);
	close(mypipe[1]);
	wait(&w);
}
