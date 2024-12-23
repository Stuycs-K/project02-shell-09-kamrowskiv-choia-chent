#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>


/*
  Args: line, arg_ary
  line is the unparsed string containing the line the user wants to run
  arg_ary is the array of strings that will store the parsed strings
  Return: void

  Using a while loop, the code uses strsep() to store each token (delimited by " ") as an element in arg_ary
  Then a null character is added to the end to terminate the string
*/
void cmdparse(char line[256], char * arg_ary[200]) {
    int i = 0;
    while (line) {
      char * tok = strsep(&line, " ");
      if (i > 0 && tok[0] == '"' && tok[strlen(tok) - 1] == '"') {
        tok[strlen(tok) - 1] = 0;
        arg_ary[i] = tok + 1;
      }
      else {
        arg_ary[i] = tok;
      }
      i++;
    }
    arg_ary[i] = 0;
}

/*
  Args: line, line_ary
  line is the unparsed string containing the lines the user wants to run separated by semicolons
  line_ary is the array of strings that will store the parsed strings
  Return: void

  Using a while loop, the code uses strsep() to store each token (delimited by ;) as an element in line_ary
  Then a null character is added to the end to terminate the string
*/
void colonparse(char line[256], char * line_ary[200]) {
    int i = 0;
    while (line) {
      line_ary[i] = strsep(&line, ";");
      i++;
    }
    line_ary[i] = 0;
}

/*
  Args: cwd
  cwd is the buffer that stores the current working directory that the shell is currently in
  Return: char *
  shortenpath() returns a pointer to the beginning of a string that contains the shortened cwd (aka the home directory in the pwd is replaced with ~)

  The code uses strstr() to find the first occurrence of the home directory in cwd
  If the home directory is not found in cwd then the code just returns a pointer to cwd
  If the home directory is found in cwd then the code just writes a ~ to cwd at the end of the home directory and returns the pointer to that location
*/
char * shortenpath(char cwd[256]) {
  char * home = getenv("HOME");
  char * p = strstr(cwd, home);
  if (p == 0) {
    return cwd;
  }
  else {
    cwd[strlen(home) - 1] = '~';
    return cwd + strlen(home) - 1;
  }
}

/*
  Args: splitinput, i
  splitinput containing the parsed input
  i is the index of the "<" symbol
  Return: void

  Redirects the input if a "<" symbol is entered, used before execvp
*/
void input_redirection(char * splitinput[200], int i) {
  int fd1 = open(splitinput[i + 1], O_RDONLY);
  if (fd1 == -1) {
    perror("open failed");
    exit(1);
  }
  dup2(fd1, STDIN_FILENO);
  splitinput[i] = NULL;
  splitinput[i + 1] = NULL;
  close(fd1);
}

/*
  Args: splitinput, i
  splitinput containing the parsed input
  i is the index of the ">" symbol
  Return: void

  Redirects the output if a ">" symbol is entered, used before execvp
*/
void output_redirection(char * splitinput[200], int i){
  int fd = open(splitinput[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if(fd==-1){
    perror("open failed");
    exit(1);
  }
  if(dup2(fd,STDOUT_FILENO)==-1){
    perror("dup2 failed");
    close(fd);
    exit(1);
  }
  splitinput[i] = NULL;
  splitinput[i + 1] = NULL;
  close(fd);
}

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

void pipe_redirection(char * input[200], int pipe, int in, int out) {
  char cmd1[256] = "";
  char cmd2[256] = "";
  // when to splice the command if theres an input redirection
  int end = (in == -1) ? pipe : in;
  for(int i = 0; i < end && input[i]; i++) {
    strcat(cmd1, input[i]);
    strcat(cmd1, " ");
  }
  if(out == -1) out = 200;
  for(int i = pipe + 1; i < out && input[i]; i++) {
    strcat(cmd2, input[i]);
    strcat(cmd2, " ");
  }
  char command[512] = "";
  strcat(command, cmd1);
  strcat(command, "| ");
  strcat(command, cmd2);
  FILE * fd = popen(command, "r");
  if (!fd) {
    perror("pipe fail");
  }
  char buffer[256];
  while(fgets(buffer, sizeof(buffer), fd)) {
    printf("%s", buffer);
  }
  pclose(fd);
}

/*
  Args: cwd
  cwd is the buffer that stores the current working directory that the shell is currently in
  Return: void

  The code stores the current working directory into cwd and then shortens the path using shortenpath()
  Then the code displays the shortened path and flushes stdout
*/

void displaycwd(char cwd[256]) {

  getcwd(cwd, 256);
  char * p = shortenpath(cwd);
  printf("%s $ ", p);
  fflush(stdout);
}

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

void runcmd(char * input[200]) {
  int input_redirect = -1;
  int output_redirect = -1;
  int pipe_redirect = -1;
  for(int i = 0; input[i]; i++) {
    if(strcmp(input[i], "<") == 0) input_redirect = i;
    if(strcmp(input[i], ">") == 0) output_redirect = i;
    if(strcmp(input[i], "|") == 0) pipe_redirect = i;
  }
  pid_t p = fork();
  if (p < 0) {
    perror("fork fail");
    exit(1);
  }
  else if (p == 0) {
    if(input_redirect != -1) {
      input_redirection(input, input_redirect);
    }
    if(pipe_redirect != -1) {
      // handle output redirection before popen()
      if(output_redirect != -1) {
        output_redirection(input, output_redirect);
      }
      pipe_redirection(input, pipe_redirect, input_redirect, output_redirect);
      exit(1);
    }
    if(output_redirect != -1) {
      output_redirection(input, output_redirect);
    }
    execvp(input[0], input);
    exit(1);
  }
  else {
    int status;
    int id = wait(&status);
  }
}
