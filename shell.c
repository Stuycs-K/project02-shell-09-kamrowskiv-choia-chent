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
#include "shell.h"

int main() {
  while (1) {
    char cwd[256];
    getcwd(cwd, 256);
    char * p = shortenpath(cwd);
    printf("%s $ ", p);
    fflush(stdout);
    char input[256];
    char * bytes = fgets(input, 256, stdin);
    char in[256];
    sscanf(input, "%[^\n]", in);
    if (strcmp(in, "exit") == 0 || bytes == 0) {
      exit(1);
    }
    char * args[200];
    split_semicolon(in,args);
    int argscounter = 0;
    while(args[argscounter]!=0) {
      char * splitinput[200];
      parse_args(args[argscounter], splitinput);
      if(strcmp(splitinput[0], "cd") == 0) {
        chdir(splitinput[1]);
      } else {
        if (!execute_pipe(splitinput)) {
          pid_t p = fork();
          if (p < 0) {
            perror("fork fail");
            exit(1);
          }
          else if (p == 0) {
            input_redirection(splitinput);
            execvp(splitinput[0], splitinput);
            exit(1);
          }
          else if (p > 0) {
            int status;
            int id = wait(&status);
          }
        }
      }
}

/*
  Args: line, arg_ary, sep
  line is the unparsed string containing the line the user wants to run
  arg_ary is the array of strings that will store the parsed strings
  sep is the string containing the delimiter
  Return: void

  Using a while loop, the code uses strsep() to store each token (delimited by sep) as an element in arg_ary
  Then a null character is added to the end to terminate the string
*/
void parse(char line[256], char * arg_ary[200], char * sep) {
    int i = 0;
    while (line) {
        arg_ary[i] = strsep(&line, sep);
        i++;
    }
    arg_ary[i] = 0;
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
  Args: splitinput
  splitinput containing the parsed input
  Return: void

  Redirects the input if a "<" symbol is entered, used before execvp
*/
void input_redirection(char * splitinput[200]) {
    int fd1 = open(splitinput[2], O_RDONLY);
    if (fd1 == -1) {
      perror("open failed");
      exit(1);
    }
    int FILENO = 0;
    int backup_stdin = dup(FILENO);
    dup2(fd1, STDIN_FILENO);
    splitinput[1] = NULL;
    splitinput[2] = NULL;

}

void stdout_redirection(char * splitinput[200]){
  int i = 0;
    while(splitinput[i] && strcmp(splitinput[i],">")!=0){
      i++;
    }
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
    if(execvp(splitinput[0],splitinput)==-1){
      perror("execvp failed");
      close(fd);
      exit(1);
    }
    close(fd);

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
*/
void runcmd(char * input[200]) {
  pid_t p = fork();
  if (p < 0) {
    perror("fork fail");
    exit(1);
  }
  else if (p == 0) {
    int redirection_flag = 0;
          for(int x = 0;input[x]!=NULL;x++){
          if (strcmp(input[x], ">") == 0) {
          stdout_redirection(input);
          redirection_flag = 1;
          break;
          }else if(strcmp(input[x],"<")==0){
            input_redirection(input);
            redirection_flag = 1;
            break;
          }
          }
          if(!redirection_flag){
            execvp(input[0],input);
          }
          exit(0);
  }
  else {
    int status;
    int id = wait(&status);
  }
}
