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

char ** parse_args(char * line, char * delimiter) {
	char * buff = calloc(100, 1);
	strcpy(buff, line);

	char ** args = calloc(100, 1);
	int i;
	for (i = 0; buff != NULL; i++)
		args[i] = strsep(&buff, delimiter);
	args[i] = NULL;

	return args;
}

char * remove_spaces(char * input) {
	// 32 is ASCII char for space
	if (input[0] == 32) {
		input++;
	}
	if (input[strlen(input) - 1] == 32) {
		input[strlen(input) - 1] = 0;
	}
	return input;
}

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
	close(fd);
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

	close(mypipe[0]);
	close(mypipe[1]);
	wait(&w);
}


int main(int argc, char * argv[]) {
	// buffer used for fgets, terminal_prompt used in getcwd() to print current directory
	char buffer[100], terminal_prompt[100]; 
	/* args used to store arguments
     ex. (with input ls -a)
	     args[0] = ls
	     args[1] = -a */
	char ** args;
	/* commands used to store list of commands separated by semicolons
	   ex. (with input ls -a; echo hi;)
	       commands[0] = ls -a
	       commands[1] = echo hi */
	char ** commands;
	// w for inputting into wait()
	int w, error, fd;
	
	// prints current directory
	printf("%s# ", getcwd(terminal_prompt, 100));

	while (1) {
		// take user input
		fgets(buffer, 100, stdin);
		// setting last char in buffer to null
		buffer[strlen(buffer) - 1] = 0;
		commands = parse_args(buffer, ";");
		// looping through each of the commands to execute them sequentially
		int i;
		for (i = 0 ; commands[i] != NULL; i++) {
			commands[i] = remove_spaces(commands[i]);
			
			// 62 is ASCII char for '>'
			if (strchr(commands[i], 62) != NULL) {
				run_out_redirect_command(commands[i]);
			}
			
			// 60 is ASCII char for '<'
			else if (strchr(commands[i], 60) != NULL) {
				run_in_redirect_command(commands[i]);
			}
			
			// 124 is ASCII char for '|'
			else if (strchr(commands[i], 124) != NULL) {
				run_pipe_command(commands[i]);
			}
			
			else {
			  args = parse_args(commands[i], " ");
				// special handling for cd and exit
				if (strcmp(args[0], "exit") == 0) {
					return 0;
				}
				if (strcmp(args[0],  "cd" ) == 0) {
					chdir(args[1]);
				}
				else {
					if (fork() == 0) {
						if (execvp(args[0], args) == -1) 
						  exit(-1);
					}
			  }
			}
			wait(&w);
		}
		printf("%s# ", getcwd(terminal_prompt, 100));
	}
}
