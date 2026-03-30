#ifndef HISTORY_H
#define HISTORY_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX_SIZE 1024

//main format of History
typedef struct history {
    char **entries;
    int size;
    int capacity;
} History;

void history_init(History *h);
void history_add(History *h, const char *cmd);
const char* history_get(History *h, int index);
void history_print(History *h);
void history_free(History *h);
char* history_expand(History *h,  char *input);
void save_history(History *h,const char *filename);
void load_history(History *h,const char *filename);


#endif