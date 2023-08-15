#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define TOKEN_MAX 32 //max size of each token, ie. path, argument
#define ARG_MAX 7 //max args to a command
#define CMD_MAX 256 //max input length
#define DEF_PATH "/bin/"
#define DEF_PATH_LEN 5

#define STDIN 0
#define STDOUT 1
#define READ_END 0
#define WRITE_END 1

typedef char bool;
#define true 1
#define false 0

#define CMD_BYTES TOKEN_MAX + ARG_MAX*TOKEN_MAX + 4
typedef struct {
    char *bin_path;
    //char bin_path[TOKEN_MAX];
    char **argument;
    //char argument[ARG_MAX][TOKEN_MAX];
    int argument_count;
} Command;


void execute(bool bg, Command* command);

void pipe_func(Command* command1, Command *command2, bool bg);

//Command** commands <=> Command *commands[MAX_CMDS]
void process(char input[CMD_MAX], Command** commands);

int search_string(char *, char);
int search_substring(char *, char *);
int string_equal_check(char *, char *);
void string_copy(char *, char *);
void string_shorten(char *, int);
void string_remove_spaces(char *);
void string_trim(char *, int);
void remove_leading_spaces(char *);
#endif
