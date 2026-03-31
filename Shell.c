#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<stdlib.h>
#include<sys/wait.h>
#include "history.h"

#define MAX_SIZE 1024

// Case-insensitive string compare
int string_compare(const char *string1 ,const char *string2)
{
    int i = 0;
    char c1,c2;

    while(string1[i] != '\0' && string2[i] != '\0')
    {
        c1 = tolower(string1[i]);
        c2 = tolower(string2[i]);

        if(c1 != c2)
            return 0;

        i++;
    }
    return string1[i] == '\0' && string2[i] == '\0';
}

//parsing function 
void parsing(char* input,char **arg)
{
    // Tokenize input
        
        int i = 0;

        char *token = strtok(input, " ");
        while(token != NULL)
        {
            if(i >= MAX_SIZE - 1) break;
            (arg[i]) = token;
            token = strtok(NULL, " ");
            i++;
        }

        arg[i] = NULL;
        return;
}


int main()
{
    History *h = malloc(sizeof(History));
    if (!h) {
        perror("malloc failed");
        return 1;
    }

    // Load history
    history_init(h);
    load_history(h, "History.txt");

    while(1)
    {
        printf("myshell> ");
        fflush(stdout);

        char input[MAX_SIZE];

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        input[strcspn(input,"\n")] = '\0';

        // Exit
        if(string_compare("exit", input))
        {
            save_history(h, "History.txt");
            history_free(h);
            break;
        }

       if(input[0] == '\0') continue;

        // Expand history
        char *expand = history_expand(h, input);

        if (!expand)
        {
            printf("Invalid reference\n");
            continue;
        }

        // Use expanded command
        strcpy(input, expand);
        free(expand);
        
        // Add to history
        if(strcmp("history",input) != 0)
        history_add(h, input);
        
        if(string_compare(input,"history"))
        {
            history_print(h);
            continue;
        }

        //Special parsing case
        char *pipe_pos = strchr(input,'|');

     if(pipe_pos != NULL)
     {
        //Enter pipe mode parsing and execution
         *pipe_pos = '\0';

         char *left = input;
         if(left[0] == '\0') continue;
         char *end = left + strlen(left) - 1 ;
         
         while(end > left && *end == ' ')
         {
         *end = '\0';
         end--;
         }
         
         char *right;
           right = pipe_pos + 1 ;
         while(*right == ' ') right++;

       

          //left string parsing
         char *args1[MAX_SIZE];


          // Tokenize input
        parsing(left,args1);


        //Right string parsing
         char *args2[MAX_SIZE];

          // Tokenize input
       parsing(right,args2);


       if(args1[0] == NULL || args2[0] == NULL)
    continue;

        //PIPES 

         int fd[2];

         if(pipe(fd) == -1)
         {
            perror("Pipe failed!");
            continue;
         }

         //First fork process for executing command and changing standard input file
         pid_t pid1 = fork();

         if(pid1 == -1)
         {
            perror("fork failed");
            close(fd[0]);
            close(fd[1]);
            continue;
         }

         //Child process 1 to make process of left string and changing file descriptor to fd[1]
         if(pid1 == 0)
         {
            dup2(fd[1],STDOUT_FILENO); //stdout maps to pipe stdout --> pipe
            close(fd[0]);
            close(fd[1]);
            execvp(args1[0],args1);
            perror(args1[0]);
            exit(1);
         }
         
            //Child process 2 to make process of reading parallely and run command of right string with connecting pipe
        pid_t pid2 = fork();

        if(pid2 == -1)
        {
        perror("fork failed");
        close(fd[0]);
        close(fd[1]);
        continue;
        }

        if(pid2 == 0)
        {
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(args2[0],args2);
        perror(args2[0]);
        exit(1);
        }
        
    //parent waiting for child process
        close(fd[0]);
        close(fd[1]);

        waitpid(pid1,NULL,0);
        waitpid(pid2,NULL,0);
        continue;
        
     
    }
        //Enter normal mode parsing and execution
     

        // Tokenize input
        char *args[MAX_SIZE];
       
        //parsing
        parsing(input,args);
    
    
        // ---------------- BUILT-IN COMMANDS ----------------

        // cd
        if(strcmp("cd", args[0]) == 0)
        {
            if(args[1] == NULL)
            {
                printf("cd: Missing argument\n");
            }
            else if(chdir(args[1]) != 0)
            {
                perror("cd");
            }
            continue;
        }

        // pwd
        if(strcmp("pwd", args[0]) == 0)
        {
            char cwd[1024];
            if(getcwd(cwd, sizeof(cwd)) != NULL)
                printf("%s\n", cwd);
            else
                perror("pwd");

            continue;
        }

        // ---------------- PROCESS CREATION ----------------

        pid_t pid = fork();

        if(pid < 0)
        {
            perror("fork failed");
            continue;
        }

        if(pid == 0)
        {
            // CHILD: execute command
            execvp(args[0], args);

            // Only runs if exec fails
            perror(args[0]);
            exit(1);
        }
        else
        {
            // PARENT: wait
            wait(NULL);
        
    }
     
}
    free(h);
    return 0;
}