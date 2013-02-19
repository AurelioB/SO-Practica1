#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Falta que pongan aqui, la identificacion de usuarios
 * Con eso, creo que esta clase ya estara completa.
 * Pero ya genero el proceso hijo y espera que se termine el proceso
 * */

void creacionProcesos();
void procesoPadre();
void procesoHijo();
char *argv[3];
int IDProceso;
int prueba = 11;

int main()
{
  //Falta que aqui, se haga la comparacion de usuarios
	printf("Hola mundo, soy el proceso: %d\n", getpid());
	printf("Presione cualquier tecla para abrir sh\n");
	printf("hubo cambios");
	getchar();
	
	creacionProcesos();
	
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
	//Para la invocacion del proceso hijo en su propia ventana
	argv[0] = "sh";
	argv[1] = "/home/bbqlol/Documentos/Práctica2/src/sh";				//Modificar el path para poder hacer pruebas
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
