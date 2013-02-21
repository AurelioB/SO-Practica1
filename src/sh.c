#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "hashtable.h"

char * askUser();
char * buffer;

int main(int argc, char **argv) {
	HashTable *table = createHashTable(100);
	sleep(30);
	//int exitCalled = 0;
	char *command;
	while(1) {
		askUser("sh> ", 100);
		//strcpy(command, buffer);
		char *nextWord;
		nextWord = strtok(buffer," "); // get first token
		int wordCount = 0;
		while (nextWord != NULL) {
			if(strcmp(nextWord, "exit") == 0) {
				exit(0);
			} else if(strcmp(nextWord, "shutdown") == 0){
				exit(123);
			} else if(strcmp(nextWord, "echo") == 0) {
				nextWord = strtok(NULL," ");
				if(nextWord != NULL && nextWord[0] == '$') {
					memmove(nextWord, nextWord + 1, strlen(nextWord));
					Var *res = findVar(table, nextWord);
						if(res == NULL)
							printf(" ERROR: Variable %s not declared.\n", nextWord);
						else
							printf(" %s\n",res->val);

				} else {
					printf(" ERROR: Expected variable query symbol ($).\n");
				}
			} else if(strcmp(nextWord, "export") == 0) {
				nextWord = strtok(NULL,"=");
					if(nextWord != NULL) {
						char *var = strdup(nextWord);
							if(nextWord != NULL) {
								//setVar(table, var, nextWord);
								//printf(" KEY %s, VAL: %s", var, nextWord);
								int pid;
								char *argv[] = {"xterm", "-e", nextWord, (char *) 0 };
								pid = execvp("xterm", argv);
							} else {
								printf(" ERROR: Missing value.\n");
							}
					} else {
						printf(" ERROR: Expected variable - value pair.\n");
						}
				}
				else if(strcmp(nextWord, "addVar") == 0) {
					nextWord = strtok(NULL, " ");
					if(nextWord != NULL)
					{
						char *var = strdup(nextWord);
						if(nextWord != NULL) {
							setVar(table, var, nextWord);
							printf("Variable %s has been added\n", nextWord);
						}
						else {
							printf("ERROR: Missing variable name\n");
						}
						getchar();
					}else
					{
						printf("ERROR: Expecte variable - value pair.\n");
					}
				}
				else{
					printf("Not a recognize command\n");
				}

				wordCount++;

				nextWord = strtok(NULL," ");

		}
	}
	freeTable(table);
	return 0;
}


char *askUser(char *question, int length) {
	char text[length];
	buffer = (char*) malloc(sizeof(char) * length);
	//printf("%s ", question);
    fputs(question, stdout);
    fflush(stdout);
    if ( fgets(text, sizeof text, stdin) != NULL ) {
     char *newline = strchr(text, '\n'); // Search new line
       if ( newline != NULL ) {
         *newline = '\0';
       }
       strcpy(buffer, text);
    }
    return "";
}
