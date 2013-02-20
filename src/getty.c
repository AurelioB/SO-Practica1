#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Falta que pongan aqui, la identificacion de usuarios
 * Con eso, creo que esta clase ya estara completa.
 * Pero ya genero el proceso hijo y espera que se termine el proceso
 * */

void creacionProcesos();
void procesoPadre();
void procesoHijo();
void loadPasswords();
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


/*
 * Son 3 funciones para poder hacer la auto-invocación
 * solo tienen que ver como compartir información, tanto de proceso a proceso
 * como de programa a programa.
 * Aqui encontre info de como se puede hacer, para que se den una idea:
 * http://stackoverflow.com/questions/2682462/c-fork-and-sharing-memory
 * 
 * En resumidas cuentas, sería a base de "Semaphores", nomas seria checar como
 * funcionan y listo.
 * */

void creacionProcesos()
{
	exit(123);
	//Para la invocacion del proceso hijo en su propia ventana
	argv[0] = "sh";
	argv[1] = "./sh";				//Modificar el path para poder hacer pruebas
	argv[2] = NULL;
	IDProceso = fork();
	
	//Probando que tambien se puede usar un switch case para el manejo de procesos
	switch(IDProceso)
	{
		//Si sale error
		case -1:printf("Error al crear proceso hijo");	 
				exit(EXIT_FAILURE);
				break;
		
		//Si es proceso hijo
		case 0: procesoHijo();
				break;
		
		//Si es proceso padre
		default : procesoPadre();
		
	}
}

void procesoPadre()
{
	
	printf("%d\n", prueba);
	waitpid(IDProceso, NULL, 0);
	printf("-- %d", prueba);
	getchar();
}

void procesoHijo()
{
	//Probando como se pasaba información, falle xD
	printf("bla bla bla %d", prueba);
	prueba = 1247654;
	execvp("xterm", argv);
	getchar();
	exit(0);
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
  		printf("Welcome\n");
  		exit(123);
  	}
}