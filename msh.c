/* Name:Sanyogita Piya
    ID :1001743446
In this assignment I wrote your own shell program, Mav shell (msh)
The code compiles without any errors or warnings
Compile command:gcc msh.c
                ./a.out
*/

//Defines and libraries:
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      
#define MAX_COMMAND_SIZE 255 // The maximum command-line size
//Mav shell only supports ten arguments in addition to command
//total=10+command(1)+newline=12
//(Req:7)
#define MAX_NUM_ARGUMENTS 12  

//Function prototype for executeProcess
int executeProcess(char** token);
/*
Function name: executeProcess
Description:forks a child and executes the command 
            if failed signals the user by printing
Parameters: char** token; the command array of strings
Return: The process ID of the executed process 

*/
int executeProcess(char** token)
{
    //Forking a new process
    //(Req:9)
    pid_t pid=fork();
    if(pid==-1)
        perror("FORK FAILED: ");
    else if(pid==0)
    {
        //we are in child now
        if (execvp(token[0],&token[0]) == -1) 
        {
            //Area where execvp fails 
            //(Req: 2)
            printf("%s: Command Not Found\n",token[0]);
            fflush(NULL);
            exit(0);
        }
    }
    else
    {
        int status;
        //we are in parent now
        waitpid(pid, &status, 0 );
        fflush(NULL);
    }
    return (pid);
}
/*
The MAIN function where program starts to execute
Return: 0 if successfully executed 
*/
int main()
{   
    //For counting PID
    int countPid=0;
    int pidArray[15];
    int pidNum;
    //To store the commands
    char* historyArray[15];
    int countHistory=0;
    //Creating space to store each command in array
    int a;
    for(a=0;a<15;a++)
    {
        historyArray[a]=(char*) malloc(MAX_COMMAND_SIZE);
    }
    
    char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
    while( 1 )
    {
        //Print msh for every new prompt
        //(Req:4)
        printf ("msh> ");

    /*  Read the command from the commandline.  The
        maximum command that will be read is MAX_COMMAND_SIZE
        This while command will wait here until the user
        inputs something since fgets returns NULL when there
        is no input */
        while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
        
        //When user enters a blank line, ignore everything below
        //and start a new prompt
        //(Req:6)
        if(cmd_str[0]=='\n')
            continue;
        
        //When user enters !n where n is the number to execute
        //that command from history should be executed
        //(Req:12)
        char *working_str  = strdup( cmd_str );   
        //if the fist element of input is !             
        if(working_str[0]=='!')
        {
            int num=atoi(&working_str[1]);
            //Since command value is 1 less than the number entered
            int commandValue=num-1;
            if(commandValue<15)
            {
                //If user enters value that is not on list
                //(Req: 12)
                if(commandValue<countHistory)
                {
                    strcpy(working_str,historyArray[commandValue]);
                    strcpy(cmd_str,historyArray[commandValue]);
                }
                else
                {
                    printf("Command not in history\n");
                    strcpy(working_str,"history");
                }
               
            }
            else
            {
                printf("Please Enter a value from 1-15\n");
                continue;
            }
        }
        //Parsing the input(Given to us)
        char *token[MAX_NUM_ARGUMENTS];
        int   token_count = 0;                                 
        char *argument_ptr;  
        char *working_root = working_str;
        // Tokenize the input strings with whitespace used as the delimiter
        while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
                token[token_count] = NULL;
            }
            token_count++;
        }
        free( working_root );
        //When the user enters exit or quit the program must end
        //(Req:5)
        if(strcmp(token[0],"exit")==0 || strcmp(token[0],"quit")==0)
        {
            return 0;
            exit(0);
        }
        //When the user enters cd command the directory should be changed
        //(Req: 10)
        else if(strcmp(token[0],"cd")==0)
        {
            //This statement is for listpids:
            //Getting the pid for cd
            pidNum=getppid();
            //if the process is 16th process then alter the array of pids
            //by moving next element to previous
            if(countPid<=14)
            {
                pidArray[countPid++]=pidNum;
            }
            else
            {
                int i;
                for(i=0;i<countPid;i++)
                {
                    pidArray[i]=pidArray[i+1];
                }
                pidArray[14]=pidNum;
            }
            //if user enters just cd go to home directory
            if(token[1]==NULL)
            {
                printf("No such file or directory is found\n");
            }
            //token[2] represents 3 arguments, in cd only 
            //2 arguments are allowed
            else if(token[2]!=NULL)
            {
                printf("Too many arguments\n");
            }
            else
            {
                if (chdir(token[1])!=0) 
                {
                    //Area where directory is not found to change to
                    printf("msh: cd: %s : No such file or directory\n",token[1]);
                }
            }
        }
        //When user enters listpids, the pid of last 15 process should be printed
        //(Req: 11)
        else if(strcmp(token[0],"listpids")==0)
        {
            //if more than one argument is given, the command shouldnt work
            if(token[1]!=NULL)
            {
                printf("Too many arguments");
            }
            else
            {
                int i;
                printf("PID LIST\n");
                for(i=0;i<countPid;i++)
                {
                    printf("%d: %d\n",i+1,pidArray[i]);
                }
                
            }
        }
        //When the user enters history, the last 15 commands should be listed
        //(Req:12)
        else if(strcmp(token[0],"history")==0)
        {
            if(token[1]!=NULL)
            {
                printf("Too many arguments");
            }
            else
            {
                int i;
                printf("COMMAND LIST\n");
                for(i=0;i<countHistory;i++)
                {
                    printf("%d: %s\n",i+1,historyArray[i]);
                }
            }
        }
        //When user enters any command it should be executed 
        //(Req:1)
        else
        {
            //function called to process the given command
            pidNum=executeProcess(token);
            //This part is for listpids:
            //Since function returns process id check
            //if the process is 16th process then alter the array of pids
            //by moving next element to previous
            if(countPid<=14)
            {
                pidArray[countPid++]=pidNum;
            }
            else
            {
                int i;
                for(i=0;i<countPid;i++)
                {
                    pidArray[i]=pidArray[i+1];
                }
                pidArray[14]=pidNum;
            }
        }
        //This part is for history command:
        //if the command  is 16th command then alter the array of commands
        //by moving next element to previous
        //if not then store the command to the array
        if(!(strcmp(token[0],"history")==0))
        {
            if(countHistory<=14)
            {
                strncpy(historyArray[countHistory++],cmd_str,MAX_COMMAND_SIZE);
            }
            else
            {
                int j;
                for(j=0;j<14;j++)
                {
                    strncpy(historyArray[j],historyArray[j+1],MAX_COMMAND_SIZE);
                }
                strncpy(historyArray[14],cmd_str,MAX_COMMAND_SIZE);
            }            
        } 
        
    }
    //End of program
    return 0;
}






