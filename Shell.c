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

        

        // Tokenize input
        char *args[MAX_SIZE];
        int i = 0;

        char *token = strtok(input, " ");
        while(token != NULL)
        {
            if(i >= MAX_SIZE - 1) break;
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        if(i == 0) continue;

        args[i] = NULL;

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