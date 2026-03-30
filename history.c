

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "history.h"




/* History part */
//We will inititiate initally memory of array
void history_init(History *h)
{

 h->capacity = 16;
 h->size = 0;
  h->entries = malloc(h->capacity * sizeof(char*));

  if(h->entries == NULL)
  {
    printf("Memory allocation failed!\n");
    return;
  }
  return;
}

//We will now add history appending as user enters
void history_add(History *h, const char *cmd)
{
    if(h->size == MAX_SIZE)
    {
        printf("Capacity Full!\n");
        return;
    }
    if(h->size == h->capacity)
    {
       int new_capacity = 2 * h->capacity;
       char **temp = realloc(h->entries, new_capacity * sizeof(char*));
if (!temp) {
    printf("Memory allocation failed!\n");
    return;
}
h->entries = temp;
h->capacity = new_capacity;
    }
    h->entries[h->size] = strdup(cmd);

    (h->size)++;

    return;

}

//Now we can access any string stored at perticular index in order 
 const char* history_get(History *h, int index)
{
    index--;
 if(index < 0 || index >= h->size)
 {
    return NULL;
 }

 return h->entries[index];
}

//Overall history displayer
void history_print(History *h)
{
    for(int i = 0 ; i < h->size ; i++)
    {
        printf("%d: %s\n",i + 1,h->entries[i]);
    }
    return;
}

//Deleting whole history basically freeing history memory
void history_free(History *h)
{
    for(int i = 0 ; i < h->size ; i++)
    {
        free(h->entries[i]);
    }
    free(h->entries);
    return;
}

//preprocessign design 
char* history_expand(History *h,  char *input)
{
  if (strcmp("!!", input) == 0)
{
    if (h->size == 0)
        return NULL;
    char *cmd = strdup(h->entries[h->size - 1]);
    if(!cmd) return NULL;
    return cmd;
}
  

    if(input[0] == '!')
    {
        int index = 0;
        
        for(int i = 1; input[i] != 0 ; i++)
        {
           if(!isdigit(input[i])) 
           { 
            return NULL;
           }
           index = index*10 + (input[i] - '0');
           
        }
        
        
    const char *cmd = history_get(h, index);
if (!cmd) return NULL;

return strdup(cmd);
    }
    char *cmd = strdup(input);
    if (!cmd) return NULL;
    return cmd;
    }

//Saving history 
void save_history(History *h,const char *filename)
{

    FILE *fptr;
    fptr = fopen(filename,"w");

    if(fptr == NULL)
    {
        perror("Error");
        return;
    }

    for(int i = 0 ; i < h->size ; i++)
    {
       fprintf(fptr,"%s\n",h->entries[i]);
    }

    fclose(fptr);
    return;

}

//Loading history
void load_history(History *h,const char *filename)
{
    FILE *fptr;
    fptr = fopen(filename,"r");
    if(fptr == NULL)
    {
        perror("Error");
        return;
    }
    char line[500];
    while(fgets(line,500,fptr) != NULL)
    {
        line[strcspn(line,"\n")] = '\0' ;
        history_add(h,line);
    }

    fclose(fptr);
    return;
}



