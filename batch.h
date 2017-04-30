
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define BUF_SIZE 1
#define BEGIN "%BEGIN"
#define END "%END"
#define INTERSTART "#INTERSTART"
#define INTERSTOP "#INTERSTOP"
#define InterStart "INTERSTART"
#define InterStop "INTERSTOP"


int checkRead(char *,int,char *);

int removeBlankSpace();

char** split(char *);

void execute_norm(char *);

void execute_interstart(char *);

int removeComment();

void clearFile(char *);


void main_module(FILE *);

void readLiner(FILE *);

void execute_instruction(char *,int);

void execute_redirect(int,int,char *,int);

void execute_normalinstruction(int,int,int);

void loggedtoFile(int,char *,int);

void childProcessExecution(int,int,int,char **,char *,int,int);

int checkBeginEnd(FILE *);