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
        while(args[argscounter]!=0){
          char * splitinput[200];


        parse_args(args[argscounter], splitinput);
      if(strcmp(splitinput[0], "cd") == 0) {
        chdir(splitinput[1]);
      } else {
        pid_t p = fork();

        if (p < 0) {
            perror("fork fail");
            exit(1);
        }
        else if (p == 0) {
            execvp(splitinput[0], splitinput);
            exit(1);
        }
        else {
            int status;
            int id = wait(&status);
        }

    }
    argscounter++;
}

}
}

void parse_args(char line[256], char * arg_ary[200]) {
    int i = 0;
    while (line) {
        arg_ary[i] = strsep(&line, " ");
        i++;
    }
    arg_ary[i] = 0;
}
void split_semicolon(char line[256], char * arg_ary[200]){
  int i = 0;
  while(line){
    arg_ary[i] = strsep(&line, ";");
    i++;
  }
  arg_ary[i] = 0;

}

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
