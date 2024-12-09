# Systems Project2 - MyShell
Andrew Choi, Victor Kamrowski, Tony Chen
Team name: Hot dog

Features:
    1) Prompting users for commands and using execvp to run them
    2) Multi-command lines with ";"
    3) Changing directories with cd and tracking directories
    4) Prompt includes the path and includes the home directory shortening using ~
    5) Exiting the prompting using "exit" and Ctrl-D
    6) Implemented simple pipes using popen()

Attempted items:
    1) Tried to incorporate using quotations to group arguments in commands but fell short when dealing with arguments with spaces in them and quotations (ie: "1 2" does work but "1" will)

Headers: 

/*
  Args: line, arg_ary
  line is the unparsed string containing the line the user wants to run
  arg_ary is the array of strings that will store the parsed strings
  Return: void

  Using a while loop, the code uses strsep() to store each token (delimited by " ") as an element in arg_ary
  Then a null character is added to the end to terminate the string
*/

/*
  Args: line, line_ary
  line is the unparsed string containing the lines the user wants to run separated by semicolons
  line_ary is the array of strings that will store the parsed strings
  Return: void

  Using a while loop, the code uses strsep() to store each token (delimited by ;) as an element in line_ary
  Then a null character is added to the end to terminate the string
*/

/*
  Args: cwd
  cwd is the buffer that stores the current working directory that the shell is currently in
  Return: char *
  shortenpath() returns a pointer to the beginning of a string that contains the shortened cwd (aka the home directory in the pwd is replaced with ~)

  The code uses strstr() to find the first occurrence of the home directory in cwd
  If the home directory is not found in cwd then the code just returns a pointer to cwd
  If the home directory is found in cwd then the code just writes a ~ to cwd at the end of the home directory and returns the pointer to that location
*/

/*
  Args: splitinput, i
  splitinput containing the parsed input
  i is the index of the "<" symbol
  Return: void

  Redirects the input if a "<" symbol is entered, used before execvp
*/

/*
  Args: splitinput, i
  splitinput containing the parsed input
  i is the index of the ">" symbol
  Return: void

  Redirects the output if a ">" symbol is entered, used before execvp
*/

/*
  Args: input, pipe, in, out
  input containing the parsed input
  pipe is the index of the "|" symbol
  in is the index of the "<" symbol
  out is the index of the ">" symbol
  Return: void

  Indexes used to determine what commands to run based on if there is a redirect symbol
  Runs popen() after using strcat to run the command.
*/

/*
  Args: cwd
  cwd is the buffer that stores the current working directory that the shell is currently in
  Return: void

  The code stores the current working directory into cwd and then shortens the path using shortenpath()
  Then the code displays the shortened path and flushes stdout
*/

/*
  Args: input
  input is the buffer that stores the command the user enters
  Return: void

  The code creates a child process and then uses execvp() to run the command on the child process
  The parent process waits for the child process to finish running
  With input and output redirection, handles the redirection before running execvp
  If there is a pipe, calls the pipe_redirection function to run popen() to execute after handling all other redirections
  Finds the indexes of the redirection symbols to use in redirection functions
*/