#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
* Falta que pongan aqui, la identificacion de usuarios
* Con eso, creo que esta clase ya estara completa.
* Pero ya genero el proceso hijo y espera que se termine el proceso
* */
void loadPasswords();
int creatSH();
char * askUser();
int validateUser();
int getNumberOfUsers();

char *argv[3];
char **passwords;
int numPasswords = 0;
int IDProceso;
int prueba = 11;

int main()
{
	//Falta que aqui, se haga la comparacion de usuarios
	printf("PID: %d\n", getpid());
	loadPasswords();
	validateUser();
	//printf("\n%s\n", askUser("Usuario: ", 20));
	//printf("%s", passwords[2]);
	//creacionProcesos();
	return 0;
}

void loadPasswords() {

	FILE *fp ;
    fp = fopen ("shadow", "r");
    
    int ch;
    int currLineNum = 0;
    int char_index = 0;
    int maxLength = 100;

    numPasswords = getNumberOfUsers();

	passwords = malloc(numPasswords * sizeof(char*));

    char* current_line = (char*) malloc(sizeof(char) * maxLength);

    while((ch = fgetc(fp)) != EOF) {

        if(ch == '\n') {

            // new line
            *(current_line + char_index) = '\0';
            char_index = 0;
            passwords[currLineNum] = current_line;
            currLineNum++;
            current_line = (char*) malloc(sizeof(char) * maxLength);

        } else if(ch != '\r') {

            // regular char
            *(current_line + char_index) = ch;
            char_index++;

        }

    }

    passwords[currLineNum] = current_line;

    fclose(fp);

}

int getNumberOfUsers() {
	FILE *fp ;
    fp = fopen ("shadow", "r");
    int ch;
    int num = 0;

	do {
		ch = fgetc(fp);
		if(ch == '\n')
			num++;
	} while (ch != EOF);

	return num;
}

char * buffer;
char * askUser(char *question, int length) {
	char text[length];
	buffer = (char*) malloc(sizeof(char) * length);
    fputs(question, stdout);
    fflush(stdout);
    if ( fgets(text, sizeof text, stdin) != NULL ) {
     char *newline = strchr(text, '\n'); /* search for newline character */
       if ( newline != NULL ) {
         *newline = '\0';
       }
       strcpy(buffer, text);
    }
    return "";
}

int validateUser() {
	int pid, pidwait;
	int status = -1;
	int length = 50;
	char login[length * 2 + 2];
	askUser("User: ", length);
	strcpy(login, buffer);
	strcat(login, ":");
	askUser("Password: ", length);
	strcat(login, buffer);
	free(buffer);
	int i;
	int result = -1;
	for(i = 0; i < numPasswords; i++) {
		result = strncmp(login, passwords[i], 102);
		//printf("TEST: %s CORRECT: %s RES: %d\n", login, passwords[i], result);
		if(result == 0) break;
   }
   
   if(result == 0) {
	    pid = creatSH();
		printf("Welcome, PID de sh = %d\n", pid);
		pidwait = wait(&status);
		status = readStatus("status");
		printf("%d", status);
		getchar();
		if(status == 0)
		{
			exit(0);
		}
		else if(status == 123)
		{
			exit(123);
		}
		
   }
}

int creatSH()
{
	int pid = fork();
	if(pid < 0)
	{
		printf("Error al crear sh\n");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{
		char *argv[] = {"sh", "echo 0 > status; ./sh; echo $? > status", (char *) 0 };
		pid = execvp("sh", argv);
	}
	
	return pid;
}

int readStatus (const char* filename)
{
	FILE *file = fopen ( filename, "r" );
	if ( file != NULL ) {
		char line [ 128 ];
		if ( fgets ( line, sizeof line, file ) != NULL ) {
			printf("LINEA %s", line);
			return (int) strtol(line, (char **)NULL, 10);
		}
	}
	return -1;
}
