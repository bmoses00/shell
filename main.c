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
	char buffer[100];
	char ** args;
	char s[100];
	int f;
	printf("%s# ", getcwd(s, 100));
	int w;
	while(1){
		fgets(buffer, 100, stdin);
		buffer[strlen(buffer)-1] = 0;
		args = parse_args(buffer);
		if (strcmp(args[0], "cd") == 0){
			chdir(args[1]);
		}
		f = fork();
		if (f == 0){
			execvp(args[0], args);
		}
		wait(&w);
		printf("%s# ", getcwd(s, 100));

		// else exit(0);
	}
	return 0;
	// buffer[strlen(dir_to_scan) - 1] = 0;
}




char * strtype(int type);
//
// int main(int argc, char * argv[]) {
//   char dir_to_scan[256];
//   if (argv[1] == NULL) {
//
//   }
//   else {
//     strcpy(dir_to_scan, argv[1]);
//   }
//   DIR * dir = opendir(dir_to_scan);
//
// }
