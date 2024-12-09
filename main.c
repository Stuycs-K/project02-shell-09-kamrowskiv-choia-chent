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
        displaycwd(cwd);
        char input[256];
        char * bytes = fgets(input, 256, stdin);
        char in[256];
        sscanf(input, "%[^\n]", in);
        if (strcmp(in, "exit") == 0 || bytes == 0) {
            exit(1);
        }

        char * args[200];
        colonparse(in,args);
        int argscounter = 0;

        while(args[argscounter]!=0){
          char * splitinput[200];
          cmdparse(args[argscounter], splitinput);
          if(strcmp(splitinput[0], "cd") == 0) {
            chdir(splitinput[1]);
          } else {
            runcmd(splitinput);
          }
          argscounter++;
        }
      }
}