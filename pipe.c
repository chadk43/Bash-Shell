#include "common.h"
#define MAX_CMDS 2

void pipe_func(Command* command1, Command* command2, bool bg)
{
    /*
      pipe_func takes in 2 command strucutres as input, each with
      their own paths and arguments, and a background process flag.
      
      pipes one process to another by opening a write end of the pipe
      and writing to it, then reading that command and using it as 
      input for the next command. 

     */
    int pipefd[2];
    int pid1,pid2;
    int child_status;
    char * const envp[]={NULL};
    //init pipe with 2 pipe ends
    pipe(pipefd);

    pid1 = fork();

    //if fork successful, write to pipe, close read end
    if (pid1 ==0)
    {
	close(pipefd[READ_END]);
	//Create copy of file descriptor called STDOUT
	dup2(pipefd[WRITE_END], STDOUT);
	//Load program into child, check if program exists
	if(execve(command1->bin_path, command1->argument, envp) == -1){
	    //Write command from child into pipe
	    write(STDOUT, command1->bin_path,TOKEN_MAX);
	    //Write error message 20 char long
            write(STDOUT, ": command not found\n", 20);
            _exit(0);
	}
    }
    
    close(pipefd[WRITE_END]);
    
    pid2 = fork();

    //Same process with new child, writing out of pipe
    if (pid2 ==0)
    {
	close(pipefd[WRITE_END]);
	dup2(pipefd[READ_END],0);
	if(execve(command2->bin_path,command2->argument,envp) == -1){
	    write(STDOUT, command2->bin_path,TOKEN_MAX);
            write(STDOUT, ": command not found\n", 20);
            _exit(0);
	}
	    
    }
    close(pipefd[READ_END]);
    //If background process required, halt child processes
    if(!bg)
    {
	waitpid(pid1,&child_status,0);
	waitpid(pid2,&child_status,0);
    }
    
}
