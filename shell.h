#ifndef SHELL_H
#define SHELL_H
void parse_args(char line[256], char * arg_ary[200]);
void split_semicolon(char line[256], char * arg_ary[200]);
char * shortenpath(char cwd[256]);
void input_redirection(char * splitinput[200]);
#endif
