#ifndef SHELL_H
#define SHELL_H
void cmdparse(char line[256], char * arg_ary[200]);
void colonparse(char line[256], char * arg_ary[200]);
char * shortenpath(char cwd[256]);
void displaycwd(char cwd[256]);
void runcmd(char * input[200]);
void input_redirection(char * splitinput[200], int i);
void output_redirection(char * splitinput[200], int i);
void pipe_redirection(char * splitinput[200], int i, int in, int out);
#endif
