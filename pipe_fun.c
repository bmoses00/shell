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

	// forking to perform 'ls'
  int f = fork();
  printf("My PID is: %d, and I am %d\n", getpid(), f);
  // crafting 'ls' command
  char * command[8];
  command[0] = "ls";
  command[1] = NULL;
  
  // changing stdout of child process to input of pipe
  if (f == 0) {	
  	dup2(mypipe[1], 1);
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
  
  // changing stdin of 'tr' to output of pipe
  if (f == 0) {
  	dup2(mypipe[0], 0);
	close(mypipe[1]);
  	execvp(command[0], command);
  }
  
  close(mypipe[0]);
  close(mypipe[1]);
  wait(&w);
  return 0;
}
