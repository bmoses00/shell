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

int main(int argc, char * argv[]) {
	char buffer[100], s[100];
	char ** args;
	char ** commands;
	char ** test;
	int fork_status, wait_status, error, temp_stdout_fileno, fd;
	printf("%s# ", getcwd(s, 100));

	while (1) {
		// take user input
		fgets(buffer, 100, stdin);
		buffer[strlen(buffer) - 1] = 0;
		// commands is now an array of Strings. Each String is a command, ex ls -l.
		commands = parse_args(buffer, ";");
		int i;
		for (i = 0 ; commands[i] != NULL; i++) {
			commands[i] = remove_spaces(commands[i]);
			args = parse_args(commands[i], " ");

			// 62 is ASCII char for '>'
			if (strchr(commands[i], 62) != NULL) {
				test = parse_args(commands[i], ">");
				args = parse_args(remove_spaces(test[0]), " ");
				char * file_name = remove_spaces(test[1]);

				fd = open(file_name, O_CREAT|O_EXCL|O_WRONLY, 0755);
				if (fd == -1) {
					fd = open(file_name, O_WRONLY);
				}

				temp_stdout_fileno = dup(1);
				dup2(fd, 1);

				fork_status = fork();
				if (fork_status == 0) {
					error = execvp(args[0], args);
					if (error == -1) return 0;
				}

				dup2(temp_stdout_fileno, 1);

			}
			else if (strchr(commands[i], 60) != NULL) {
				test = parse_args(commands[i], "<");
				args = parse_args(remove_spaces(test[0]), " ");
				char * file_name = remove_spaces(test[1]);

				fd = open(file_name, O_RDONLY);

				temp_stdout_fileno = dup(0);
				dup2(fd, 0);

				fork_status = fork();
				if (fork_status == 0) {
					error = execvp(args[0], args);
					if (error == -1) return 0;
				}

				dup2(temp_stdout_fileno, 0);

			}
			else {
				// special handling for cd and exit
				if (strcmp(args[0], "exit") == 0) {
					return 0;
				}
				if (strcmp(args[0],  "cd" ) == 0) {
					chdir(args[1]);
				}
				else {
					// forking and executing commands
					fork_status = fork();
					if (fork_status == 0) {
						error = execvp(args[0], args);
						if (error == -1) return 0;
					}
			  }
			}
			wait(&wait_status);
		}
		printf("%s# ", getcwd(s, 100));
	}
}
