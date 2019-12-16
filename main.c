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
# include "executes.h"
# include "parse.h"

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

		  if (strchr(commands[i], 62) != NULL
						&& strchr(commands[i], 60) != NULL) {
				run_in_out_redirect_command(commands[i]);
			}
			// 62 is ASCII char for '>'
			else if (strchr(commands[i], 62) != NULL) {
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

					free(commands[0]);
					free(commands);
					free(args[0]);
					free(args);

					return 0;
				}
				if (strcmp(args[0],  "cd" ) == 0) {
					chdir(args[1]);
					free(args[0]);
					free(args);
				}
				else {
					run_regular_command(args);
			  }
			}
			wait(&w);
		}
		// we only need to free commands[0] if there is a
		// semicolon (if i > 0)
		if (i > 0) {
			free(commands[0]);
		}
		free(commands);

		printf("%s# ", getcwd(terminal_prompt, 100));
	}
}
