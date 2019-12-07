# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <errno.h>
# include <unistd.h>
# include <time.h>
# include <sys/wait.h>

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
	int f, w, e;
	printf("%s# ", getcwd(s, 100));

	while (1) {
		// take user input
		fgets(buffer, 100, stdin);
		buffer[strlen(buffer) - 1] = 0;
		commands = parse_args(buffer, ";");
		// args is now an array of Strings. Each String is a command, ex ls -l.
		int i;
		for (i = 0 ; commands[i] != NULL; i++) {
			commands[i] = remove_spaces(commands[i]);
			args = parse_args(commands[i], " ");
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
					e = execvp(args[0], args);
					if (e == -1) return 0;
				}
		  }
			wait(&w);
		}
		printf("%s# ", getcwd(s, 100));
	}

}
