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
      parse_args(in, args);

      pid_t p = fork();

      if (p < 0) {
          perror("fork fail");
          exit(1);
      }
      else if (p == 0) {
          if
          execvp(args[0], args);
          exit(1);
      }
      else {
          int status;
          int id = wait(&status);
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
