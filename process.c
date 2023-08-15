#include "common.h"

/* makes a command by splitting input into program path and arguments, and
 * formats it correctly for input into execve()
 *
 * Command* command - Requires every char of every member string to be '\0'
 */
void make_command(char input[CMD_MAX], Command* command) {
	int input_index = 0;
	int k = 0;
	char ch;
	bool bin_done = false; //program path is processed
	int path_add = 0;

	command->argument_count = 0;

	ch = input[0];

	if (ch != '/') {
            //assume user not typing absolute path
            string_copy(DEF_PATH, command->bin_path);                
			path_add = DEF_PATH_LEN;
    }

	while (ch != '\n') {
		if (!bin_done) {
			if (ch != ' ') {
				/* put input path into both command path, and 
				 * first arg by convention */
				command->bin_path[input_index+path_add] = ch;
				command->argument[0][input_index] = ch;
			}
			else {
				command->bin_path[input_index+path_add] = '\0';
				command->argument[0][input_index] = '\0';
				bin_done = true;
			}
		//form arguments
		} else if (ch != ' ') {
			command->argument[command->argument_count+1][k] = ch;
			k++;
		} else {
			command->argument[command->argument_count+1][k] = '\0';
			command->argument_count++;
			k = 0;
		}		
		ch = input[++input_index];
	}
	command->argument[command->argument_count+1][k] = '\0';
	command->bin_path[input_index+path_add] = '\0';

	//replace every empty argument to point to NULL as required by execve()
	for (int i = 1; i < ARG_MAX; i++) {
		if (command->argument[i][0] == '\0') {
			command->argument[i] = NULL;
		}
	}
}

/* taking input, splits commands by pipe and forms a structure for each
 *
 * Command** commands - Expects array of pointers to commands of size MAX_CMDS
 */
void process(char input[CMD_MAX], Command** commands) {
	char string[CMD_MAX] = {'\0'}; 
	int pipe_in;
	int cmd1_end;
	int i;
	bool invalid = false;

	pipe_in = search_string(input, '|');
	if (pipe_in < 0) {
		//no pipe
		make_command(input, commands[0]);
	}
	else {
		cmd1_end = pipe_in - 1;
		if (cmd1_end < 0) {
			//pipe with no input
			invalid = true;
		}
		else {
			while (input[cmd1_end] == ' ') {
				//consume whitespace between first command
				//and pipe
				cmd1_end--;
			}
			for(i = 0; i <= cmd1_end; i++) {
				string[i] = input[i];
			}
			/* string is terminated with '\n' instead of '\0'
			 * because that is what make_command() expects as end
			 * of input */
			string[i] = '\n';

			pipe_in++;
			if (input[pipe_in] == '\0') {
				//no pipe output
				invalid = true;
			}
			else {
				while(input[pipe_in] == ' ') {
					pipe_in++;
				}
			}
		}
		if (!invalid) {
			make_command(string, commands[0]);

			//pointer arith. to get start of second cmd
			make_command(input + pipe_in, commands[1]);
		}
		else {
			write(STDOUT, "Invalid pipe\n", 13);
		}
	}
}
