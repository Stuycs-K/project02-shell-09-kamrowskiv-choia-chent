#ifndef SHELL_H
#define SHELL_H
void parse(char line[256], char * arg_ary[200], char * sep);
char * shortenpath(char cwd[256]);
void displaycwd(char cwd[256]);
void runcmd(char * input[200]);
#endif
