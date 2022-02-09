# Mav-Shell
## Create a shell program like bash that accepts commands, fork a child process, and execute those commands. 
The shell command will prompt as msh>
Program will print out a prompt of 
msh> 
when it is ready to accept input.
If the command is not supported your shell shall print the invalid command followed by “: Command not found.”
If the command option is an invalid option then the shell shall print the command followed by “: invalid option --” and the option that was invalid as well as a prompt to try  help.
After each command completes, the program shall print the msh> prompt and accept another line of input
The shell will exit with status zero if the command is “quit” or “exit”. 
