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

// ADD FREEING, CLOSING FILES

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
	// buffer used for fgets, terminal_prompt used in getcwd() to print current directory
	char buffer[100], terminal_prompt[100]; 
	/* args used to store arguments
     ex. (with input ls -a -l)
	     args[0] = ls
	     args[1] = -a 
	     args[2] = -l */
	char ** args;
	/* commands used to store list of commands separated by semicolons
	   ex. (with input ls -a; echo hi;)
	       commands[0] = ls -a
	       commands[1] = echo hi */
	char ** commands;
	
	char ** test; // rename later! (used to store parsed args for > and <)
	
	// f for checking fork(), w for inputting into wait()
	int f, w, error, fd;
	
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

				f = fork();
				if (f == 0) {
				  dup2(fd, 1);
					error = execvp(args[0], args);
					if (error == -1) return 0;
				}

			}
			// 60 is ASCII char for '<'
			else if (strchr(commands[i], 60) != NULL) {
				test = parse_args(commands[i], "<");
				args = parse_args(remove_spaces(test[0]), " ");
				char * file_name = remove_spaces(test[1]);

				fd = open(file_name, O_RDONLY);

				f = fork();
				if (f == 0) {
					dup2(fd, 0);
					error = execvp(args[0], args);
					if (error == -1) return 0;
				}

			}
			// 124 is ASCII char for '|'
			else if (strchr(commands[i], 124) != NULL) {
				test = parse_args(commands[i], "|");
				char ** first_command = parse_args(remove_spaces(test[0]), " ");
				char ** second_command = parse_args(remove_spaces(test[1]), " ");
				
				int mypipe[2];
				pipe(mypipe);
				
				int f = fork();
				if (f == 0) {
				  dup2(mypipe[1], 1);
  				execvp(first_command[0], first_command);
				}
				
				int w;
				wait(&w);
				
				f = fork();
				if (f == 0) {
  				dup2(mypipe[0], 0);
					close(mypipe[1]);
  				execvp(second_command[0], second_command);
  			}
  
  			close(mypipe[0]);
  			close(mypipe[1]);
  			wait(&w);
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
					f = fork();
					if (f == 0) {
						error = execvp(args[0], args);
						if (error == -1) return 0;
					}
			    }
			}
			wait(&w);
		}
		printf("%s# ", getcwd(terminal_prompt, 100));
	}
}

