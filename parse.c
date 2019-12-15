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

char ** parse_args(char * line, char * delimiter) {
	char * buff = calloc(420, 1);
	strcpy(buff, line);

	char ** args = calloc(125, 1);
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
