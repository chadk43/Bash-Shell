#include <unistd.h>
#include <stdio.h>
#include "common.h"

void read_cmd(char *);  
void mem_manage(Command** commands, bool *s);
void reset_cmds(Command** commands);


#define MAX_CMDS 2

int main() {

    bool exit = false;
    bool bg;
    int bg_index, redirect_out_index, redirect_in_index,
	ofd,ifd,
	saved_out, saved_in;
    char exit_cmd[] = "exit";
    char input[CMD_MAX];
    char temp[CMD_MAX];
    char temp1[CMD_MAX];
    char input_file_name[TOKEN_MAX];
    char output_file_name[TOKEN_MAX];
    bool struct_alloc = false;
    Command* commands[MAX_CMDS];
	
    read_cmd(input);
	//Exit when 'exit' is input
    exit = string_equal_check(exit_cmd,input);
	
    while (!exit) {
	//Look for background process flag (bg)
	bg_index = search_string(input, '&');
	//Look for file redirection flags
	redirect_out_index = search_string(input, '>');
	redirect_in_index = search_string(input, '<');
	
	
	if(bg_index != -1){
	    bg = true;
	    string_shorten(input,bg_index);
	}else {
	    bg = false;
	}
	
	//if input is non empty, create new command strucutre and set initial fields to blank
	if (input[0] != '\n'){ 
	    mem_manage(commands, &struct_alloc);
	    reset_cmds(commands);

	    if(redirect_out_index != -1){
		saved_out = dup(STDOUT);
		//If the input redirection symbol comes after the output redirection symbol, 
		//the input symbol will be replaced by a null terminator so the input does not
		//get copied into the output file name.
		if(redirect_in_index > redirect_out_index){
		    input[redirect_in_index] = '\0';
		}
		string_copy(&input[redirect_out_index + 1],output_file_name);
		
		//Check if the input comes after the output. This is because if the input is after,
		//trimming at the output's symbol will cut off the input.
		if(redirect_in_index < redirect_out_index){
		    string_trim(input,redirect_out_index);
		    input[redirect_out_index] = '\n';
		}
		string_remove_spaces(output_file_name);
		//Open output file with root, owner and group all having access to read and write to file
		//0666 is linux file permission
		ofd = open(output_file_name,O_CREAT | O_RDWR, 0666);
		dup2(ofd,STDOUT);
	    }

	    if(redirect_in_index != -1){		
		saved_in = dup(STDIN);
		string_copy(&input[redirect_in_index + 1],input_file_name);

		//Depending on what the first symbol is after the command, trim the input
		//at the appropriate point.
		if(redirect_in_index > redirect_out_index && redirect_out_index != -1){
		    string_trim(input,redirect_out_index);
		    input[redirect_out_index] = '\n';
		}else{
		    string_trim(input,redirect_in_index);
		    input[redirect_in_index] = '\n';
		}
		string_remove_spaces(input_file_name);
		
		//Open input file, 0666 sets permissions in unix.
		ifd = open(input_file_name,O_RDWR, 0666);
		dup2(ifd,STDIN);
	    }

	    remove_leading_spaces(input);
	    process(input, commands);

	    //If pipe input found, pipe bin paths on either side of pipe
	    if(search_string(input,'|') != -1){
		pipe_func(commands[0], commands[1],bg);
	    }
	    else{
		//If no pipe, execute the single command
		execute(bg, commands[0]);
	    }
	}
	if(redirect_in_index != -1){
	    close(ifd);
	    dup2(saved_in,STDIN);
	}
	if(redirect_out_index != -1){
	    close(ofd);
	    dup2(saved_out,STDOUT);
	}
	mem_manage(commands, &struct_alloc);
	read_cmd(input);
	exit = string_equal_check(exit_cmd,input);
    }

    return 0;
}
/*Clean out created command structures by setting bin path
and argument fields to blank
*/
void reset_cmds(Command** commands) {
    for (int i = 0; i < MAX_CMDS; i++) {
	for (int j = 0; j < TOKEN_MAX; j++) {
	    commands[i]->bin_path[j] = '\0';
	}
	for (int j = 0; j < ARG_MAX; j++) {
	    for (int k = 0; k < TOKEN_MAX; k++) {
		commands[i]->argument[j][k] = '\0';
	    }
	}
	commands[i]->argument_count = 0;
    }
}

/* allocate structure memory; then allocate memory for *bin_path; then allocate
 * total memory for **argument, and then allocate memory for string in each
 * token, and set struct_alloc to true. When it isn't, free the same memory in
 * reverse. */
void mem_manage(Command** commands, bool *s) {
    int k;

    for(int i = 0; i < MAX_CMDS; i++) {
	if (!*s){
	    commands[i] = malloc(CMD_BYTES);
	    commands[i]->bin_path = malloc(TOKEN_MAX);
	    commands[i]->argument = malloc(ARG_MAX*TOKEN_MAX);
	    for (k = 0; k < ARG_MAX; k++) {
		commands[i]->argument[k] = malloc(TOKEN_MAX);
	    }
	}
	else {
	    free(commands[i]->bin_path);
	    for (k = 0; k < ARG_MAX; k++) {
		free(commands[i]->argument[k]);
	    }
	    free(commands[i]->argument);
	    free(commands[i]);
	}
    }
    *s = !(*s);
}
/*Reads the text from the user into a string, and ends with a null terminator. */
void read_cmd(char * input){

    for (int i = 0; i < CMD_MAX; i++) {
	input[i] = '\0';
    }
    write(STDOUT, "mysh$ ", 6);  
    read(STDIN, input, 255);  
}
