# COMP-3659-Project-1  
Mark, Daniel, and Chad's project repository for Marc Shröder's COMP-3659 class, Fall 2021  
  
Making a shell.  

Needs Basic Functionality:  

    [ ] repeatedly prompts user, sentinel loop! (while command is not exit)  
      
    [ ] read a command line from standard input (storing in a buffer of char)  
            regular grammer that describes the abstract suntax of the command line.  
            going to be a command name, whitespace(maybe multiple) then command line arguments(0 or more).  
            tokenize the buffer, group into whitespace separated words where whitespace is dropped.  
            GETTING SIMPLEST WORKING VERSION IS YOUR FRIEND!!!  
            generate a "command " struct that captures command line and 0 or more command line arguments.  
            DO THIS SO GROUP CAN WORK ON EVERYTHING CONCURRENTLY AS A TEAM!!!  
              
    [ ] cause the read commands to be executed (process command line) 
            Now we can start work on this because we know what the command structures will look like.  
              
    [ ] continue indefinitely until exited (read another command line)  
    [ ] run valid external commands (separate executable)  
    [ ] run interactive login shell  
    [x] NO EMACS OR COMMAND HISTORY NEEDED  
    [ ] 256 character limit  
Background Jobs:  

    [ ] must be able to run commands in background by ending with &  
Basic Command Pipelines  

    [ ] Connect output of one program to the next (at least 2, hopefully more)  
Basic I/O Redirection  

    [ ] Redirection (to and from files?)  
  
  
