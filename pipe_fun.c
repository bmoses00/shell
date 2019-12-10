# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>


int main() {
	// desired pathing: 
	
	// stdin     stdout      stdin     stdout
	// -- --> ls -- -->  foo -- --> wc -- --> terminal
	
	// creating our intermediary file, foo
  char * file_name = "foo";
  int fd = open(file_name, O_CREAT|O_EXCL|O_RDWR, 0755);
  if (fd == -1) {
    fd = open(file_name, O_RDWR);
  }

	// forking to perform 'ls'
  int f = fork();
  
  // crafting 'ls' command
  char * command[8];
  command[0] = "ls";
  command[1] = NULL;
  
  // changing stdout of child process to foo. Output of 'ls' will now write to foo.
  // then, execvp 'ls'.
	if (f == 0) {
		int stdout_fileno = 1;
  	int temp_stdout_fileno = dup(stdout_fileno);
  	dup2(fd, stdout_fileno);
  	execvp(command[0], command);
  }
  
  // wait for child process to finish
  int w;
  wait(&w);
  
  // move foo's cursor to beginning of file
  lseek(fd, 0, SEEK_SET);
  
  // creating child process to run 'tr'
  f = fork();
  
  // crafting 'tr' command
  command[0] = "tr";
  command[1] = "a-z";
  command[2] = "A-Z";
  command[3] = NULL;
  
  // changing stdin of 'tr' to foo
  if (f == 0) {
    int stdin_fileno = 0;
  	int temp_stdin_fileno = dup(stdin_fileno);
  	dup2(fd, stdin_fileno);
  	execvp(command[0], command);
  }
  
  // wait for child process to finish, then exit the program
  wait(&w);
  return 0;
  
}
