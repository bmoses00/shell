# incepShell
by Mohidul Abedin and Brian Moses

Features:
	Forks and executes commands!
	Parses multiple commands on one line!
	Redirects using (> , <)!
  PIPESSSSS

Attempted:
  We have no ambition

~~Bugs~~ Features (Continued):
	Putting two ;'s next to each other will stop the parser (for user's own good")
  Putting multiples spaces after a command will error
	
Files & Function Headers:
parse.c
	Handles all line parsing fucntions
	/*======== int count_tokens() ==========
	Inputs:  char *line
        	  char delimiter  
	Returns: a 2-D array refering to parsed args
  
  Seperates a line into multiple commands 
	====================*/

	/*======== char * remove_spaces(char * input) ==========
	Inputs:  char *input 
	Returns: Pointer to the beginning of line

	Removes leading and trailing whitespace on the string line.
	Terminating '\0' is placed at a new location if necessary.
	====================*/
executes.c  
  	Executes commands involving < > and |
	
	/*======== run_out_redirect_command(char * command) ==========
	Inputs:  char *command
	Returns: N/A
  
	Executes a command with forking if the command has ">"  
	====================*/

	/*======== run_in_redirect_command(char * command)  ==========	
  	Inputs:  char *command
	Returns: N/A

	Executes a command with forking if the command has "<"  
	====================*/

	/*======== run_pipe_command(char * command)  ==========
	Inputs:  char *command 
	Returns: N/A
	
	Executes a command with forking if the command has "|"  
	====================*/


main.c
	Handles the forking an executing of commands
	
