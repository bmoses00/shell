# incepShell
by Mohidul Abedin and Brian Moses

* Features:
	* Forks and executes commands!
	* Parses multiple commands on one line!
	* Redirects using (> , <)!
	* Double redirects!
	* PIPESSSSS!

* Attempted:
	* We looked at tab and arrows, but there is no official C library to implement receiving data before enter key is pressed. 

* ~~Bugs~~ Features (cont'd):
	* Putting two ;'s next to each other will stop the parser (for user's own good)
	* Putting multiples spaces after a command will error
	* Double redirecting with a non-existant INPUT file might explode your pc ("will create the output file like a good boi")
	
Files & Function Headers: <br>
parse.c <br>
	Handles all line parsing fucntions
	
	/*======== char ** parse_args() ==========
	Inputs:  char * line
        	 char * delimiter  
	Returns: a 2-D array refering to parsed args
  
  	Seperates a line into multiple commands 
	====================*/

	/*======== char * remove_spaces(char * input) ==========
	Inputs:  char *input 
	Returns: Pointer to the beginning of line

	Removes leading and trailing whitespace on the string line.
	Terminating '\0' is placed at a new location if necessary.
	====================*/
executes.c  <br>
  	Executes commands involving < > and |
	
	/*======== run_out_redirect_command(char * command) ==========
	Inputs:  char *command
	Returns: void
  
	Executes a command with forking if the command has ">"  
	====================*/

	/*======== run_in_redirect_command(char * command)  ==========	
  	Inputs:  char *command
	Returns: void

	Executes a command with forking if the command has "<"  
	====================*/

	/*======== run_pipe_command(char * command)  ==========
	Inputs:  char *command 
	Returns: void
	
	Executes a command with forking if the command has "|"  
	====================*/
	
	/*======== run_in_out_command(char * command)  ==========
	Inputs:  char *command 
	Returns: void
	
	Executes a command with forking if the command has "<" and ">"  
	====================*/
	
	/*======== run_regular_command(char ** args)  ==========
	Inputs:  char ** args
	Returns: void
	
	Executes a command with forking if the command has none of the above symbols ("<", ">", or "|") 
	====================*/


main.c <br>
	Calls functions in executes.c as necessary, and runs "exit" and "cd"
	
