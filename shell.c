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
        printf("%s $ ", cwd);

        char input[256];
        fgets(input, 256, stdin);
        char in[256];
        sscanf(input, "%[^\n]", in);
        char * args[200];
        split_semicolon(in,args);
        int argscounter = 0;
        while(args[argscounter]!=0){
          char * splitinput[200];


        parse_args(args[argscounter], splitinput);

      if(strcmp(args[0], "cd") == 0) {
        chdir(args[1]);
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

}}

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
