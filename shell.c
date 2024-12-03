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
    char input[256];
    char cwd[256];
    getcwd(cwd, 256);
    printf("%s $ ", cwd);
    while (fgets(input, 256, stdin)) {
        char withoutnl[256];
        sscanf(input, "%[^\n]", withoutnl);
        char * args[20];
        parse_args(withoutnl, args);
        pid_t p = fork();
        if (p < 0) {
            perror("fork fail");
            exit(1);
        } else if (p == 0) {
            execvp(args[0], args);
        }
        else {
            int status;
            int id = wait(&status);
            printf("%s $ ", cwd);
        }

        
    }
}

void parse_args(char line[256], char * arg_ary[20]) {
    int i = 0;
    while (line) {
        arg_ary[i] = strsep(&line, " ");
        i++;
    }
}