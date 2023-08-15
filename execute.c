#include "common.h"

void execute(bool bg, Command* command) {
/*execute creates a duplicate child process with fork, and then
loads a program into the child with execve

Additionally stalls child process if background processing was requested
 */    
    
    pid_t pid;
    int child_status;
    char * const newenvp[] = {NULL};
    
    //Create child process
    pid = fork();
    //Check if child is created
    if(pid == 0){
	//loading program specified by bin path and arguments to child process
	if(execve(command->bin_path, command->argument,newenvp)== -1){
	    write(STDOUT, command->bin_path,TOKEN_MAX);
	    write(STDOUT, ": command not found\n", 20);
	    _exit(0);
	}
    }
    //If background process tag stall child process
    if(!bg){
	waitpid(pid,&child_status,0);
    }
    
    
    return;
}
