#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "hashtable.h"

char * askUser();
int fileExists();
char * buffer;

int replaceVariables();

int main(int argc, char **argv) {
	HashTable *table = createHashTable(100);
	//int exitCalled = 0;
	setVar(table, "PATH", "");
	//setVar(table, "lele", "2");
	//setVar(table, "lili", "3");
	char *command;
	while(1) {
		askUser("sh> ", 100);
		//strcpy(command, buffer);
		if(strlen(buffer) == 0 || replaceVariables(table) == -1)
			continue;
		//printf("REPLACE: %d\n", replaceVariables(table));

		char *nextWord;
		nextWord = strtok(buffer," "); // get first token
		int wordCount = 0;
		if (nextWord != NULL) {
			if(strcmp(nextWord, "exit") == 0) {
				exit(0);
			} else if(strcmp(nextWord, "shutdown") == 0){
				exit(123);
			} else if(strcmp(nextWord, "echo") == 0) {
				nextWord = strtok(NULL,"\n");
				printf("	%s\n",nextWord);
			} else if(strcmp(nextWord, "export") == 0) { 
				nextWord = strtok(NULL,"=");
				if(nextWord != NULL) {
					char *var = strdup(nextWord);
					nextWord = strtok(NULL, " ");
					if(nextWord != NULL) {
						setVar(table, var, nextWord);
						//printf("	KEY %s, VAL: %s", var, nextWord);
					} else {
						printf("	ERROR: Missing value.\n");
					}
					
				} else {
					printf("	ERROR: Expected variable - value pair.\n");
				}
			} else {	// Execute process
				char *argv[100];
				int nArgv = 0;
				int inBackground = 0;
				while (nextWord != NULL) {
					argv[nArgv++] = strdup(nextWord);
					nextWord = strtok(NULL," ");
					//nArgv++;
				}
				if(argv[nArgv - 1][0] == '&') {
					argv[nArgv - 1] = (char *) 0;
					inBackground = 1;
				} else
					argv[nArgv] = (char *) 0;
				if(fork() == 0) {
					Var *res = findVar(table, "PATH");
					if(execvp(argv[0], argv) == -1)
							execvp(strcat(res->val,argv[0]), argv);
				} else if(!inBackground) {
            		int pid, status;
            		
            		pid = wait(&status);
        		}
				
			}

		}
	}



	freeTable(table);
	return 0;
}


char *askUser(char *question, int length) {
	char text[length];
	buffer = (char*) malloc(sizeof(char) * length);
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


int replaceVariables(HashTable *table) {
	if(strchr(buffer, '$') == NULL)
		return 0;
    char *end_str;
    char *token = strtok_r(buffer, " ", &end_str);

	char result[200];
	strcpy(result, "");

    while (token != NULL)
    {
        char *end_token;
        
        if(strchr(token, '$') == NULL) {
        	//printf("a = %s\n", token);
        	strcat(result, token);
        	strcat(result, " ");
        	token = strtok_r(NULL, " ", &end_str);
        	continue;
    	} else if(token[0] != '$') {
    		char *notVar = strtok(strdup(token), "$");
    		//printf("a = %s\n", notVar);
    		strcat(result, notVar);
    		token = strchr(token, '$');
    	}
        char *token2 = strtok_r(token, "$", &end_token);
        while (token2 != NULL) {
        	
        	/*strcat(result, "--");
        	strcat(result, token2);
        	strcat(result, "--");*/
        	Var *res = findVar(table, token2);
        	if(res == NULL) {
        		printf("	ERROR: Variable \"%s\" not declared.\n", token2);
        		return -1;
        	}
        	strcat(result, res->val);
            //printf("	b = %s\n", token2);
            token2 = strtok_r(NULL, "$", &end_token);
        }
        strcat(result, " ");
        token = strtok_r(NULL, " ", &end_str);
    }

    //printf("**\n%s\n**\n", result);
    //free(buffer);
    buffer = strdup(result);
	return 1;
}

int fileExists(const char * filename) {
	FILE * file;
    if(file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}