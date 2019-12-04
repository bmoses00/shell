# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <errno.h>
# include <unistd.h>
# include <time.h>

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

int main(int argc, char * argv[]) {
	char buffer[100];
	char ** args;
	// if (argv[1] != NULL) {
	//   char ** args = parse_args(argv[1]);
	// 	int i;
	// 	execvp(args[0], args);
	// }
	printf("$ ");
	fgets(buffer, 256, stdin);
	// printf("%s\n", buffer );
	args = parse_args(buffer);
	int i;
	for(i = 0; args[i] != NULL; i++){
		printf("args of %d is %s\n", i, args[i]);
	}
	execvp(args[1], args);
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
