#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<stdlib.h>
#include<sys/wait.h>

#define MAX_SIZE 1024

//String compare function checks weather same or not incase sensitive
int string_compare(const char *string1 ,const char *string2)
{
    int i = 0;
    char c1,c2;
    while(string1[i] != '\0' && string2[i] != '\0')
    {
        c1 = tolower(string1[i]);
        c2 = tolower(string2[i]);
      if(c1 != c2)
      {
        return 0;
      }
      i++;
    }
    return string1[i] == '\0' && string2[i] == '\0';

}


int main()
{

    while(1)
    {
        printf("myshell>");
        
        char input[MAX_SIZE];

         //getting input into shell
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input,"\n")] = '\0' ;

        if(string_compare("exit",input) == 1)
        {
            break;
        }

        //parsing
        char *token = strtok(input," ");
        int i = 0;
        char *args[MAX_SIZE];
        while(token != NULL)
        {
            if(i >= MAX_SIZE - 1) break;
           args[i++] = token;
           token = strtok(NULL," ");
        }
        if(i == 0) continue;
        args[i] = NULL ; 

        //build-ins
        if(strcmp("cd",args[0]) == 0)
        {
        if(args[0] == NULL)
        {
          printf("cd: Missing argument\n");
        }
        else if(mkdir(args[0]) != 0)
        {
            perror("cd");
        }
        else{
            mkdir(args[0]);          
        }
        continue;
         }
          if(strcmp(args[0],"pwd") == 0)
          {
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            printf("%s\n",cwd);
          }

// ------------------------------------------------------------------
        //process ID's
        pid_t pid = fork();

       if(pid < 0)
       {
        perror("fork failed");
       }

        if(pid == 0)//child process
        {
            //CHILD: program execute
            execvp(args[0],args);
            perror(args[0]);
            exit(1);
        }
        else
        {
            //Parent wait for child
            wait(NULL);
        }
       
    }

    return 0;
}