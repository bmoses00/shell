# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>


int main() {
	// desired pathing: 
	
	// stdin     stdout        stdin     stdout
	// -- --> ls -- -->  PIPE! -- --> wc -- --> terminal
	
	
  // creating the pipe
  int mypipe[2];
  pipe(mypipe);
  int fd = mypipe[1];

	// forking to perform 'ls'
  int f = fork();
  printf("My PID is: %d, and I am %d\n", getpid(), f);
  // crafting 'ls' command
  char * command[8];
  command[0] = "ls";
  command[1] = NULL;
  
  // changing stdout of child process to input of pipe
	if (f == 0) {
		int stdout_fileno = 1;
  	int temp_stdout_fileno = dup(stdout_fileno);
  	dup2(fd, stdout_fileno);
  	execvp(command[0], command);
  }
  
  // wait for child process to finish
  int w;
  wait(&w);
  
  // creating child process to run 'tr'
  f = fork();
  printf("My PID is: %d, and I am %d\n", getpid(), f);
  // crafting 'tr' command
  command[0] = "tr";
  command[1] = "a-z";
  command[2] = "A-Z";
  command[3] = NULL;
  
  fd = mypipe[0];
  // changing stdin of 'tr' to output of pipe
  if (f == 0) {
    int stdin_fileno = 0;
  	int temp_stdin_fileno = dup(stdin_fileno);
  	dup2(fd, stdin_fileno);
  	execvp(command[0], command);
  }
  
  wait(&w);
  return 0;
}
