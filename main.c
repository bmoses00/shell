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

char ** parse_args(char * line) {
	char ** args = calloc(48, 1);
	int i;
	for (i = 0; line != NULL; i++) {
		args[i] = calloc(8, 1);
		args[i] = strsep(&line, " ");
	}
	args[i] = NULL;
	return args;
}

char *** parse_comms(char * line){
	char *** comms = calloc(100, 1);
	int i;
}

int main(int argc, char * argv[]) {
	char buffer[100], s[100];
	char ** args;
	int f, w;
	printf("%s# ", getcwd(s, 100));
	
	while (1) {
		// take user input
		fgets(buffer, 100, stdin);
		buffer[strlen(buffer)-1] = 0;
		args = parse_args(buffer);
		// special handling for cd and exit
		if (strcmp(args[0], "exit") == 0) return 0;
		if (strcmp(args[0],  "cd" ) == 0) chdir(args[1]);
		else {
			// forking and executing commands
			f = fork();
			if (f == 0)
				execvp(args[0], args);
	  }
		wait(&w);
		printf("%s# ", getcwd(s, 100));
	}

}
