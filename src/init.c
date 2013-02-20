#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Perdon por el abuso de comentarios aqui, pero quiero que este tenga todo super explicado
 * despues lo borran, o lo dejan como apoyo para cuando sea la revicion en clase.
 * Esta clase, segun yo ya esta completa
 * Ya genera los 6 hijos de golpe que es lo unico que realmente debe hacer la clase init
 * */

int main()
{
  //Declaración de variables
	int i = 0;															//Contador para la generacion de los 6 procesos
	int IDprocesos;														//La variable que tendra el ID de los procesos que se van creando
				//Variable de entrada para la funcion execvp
	
	/*
	 * Ciclo encargado de crear los sub-procesos
	 * */
	do 																	
	{
		IDprocesos = fork();											//Genero el nuevo proceso
		if(IDprocesos < 0)												//Manejador de error
		{
			printf("Hubo un error de fork, regreso un ID de: %d\n", IDprocesos);
			exit(EXIT_FAILURE);
		}
		else if(IDprocesos == 0)										//Si el ID es de un hijo, ejecutar getty (ID == 0)
		{
			char *argv[] = {"xterm", "-e", "./getty", (char *) 0 };
																		/*
																		* Explacación algo larga, pero sencilla, para el uso del execvp + xterm -e "nombre del programa" 
																		* se puede ser dentro de un mismo apuntador, pero debe seguir el siguiente order.
																		* Primero, se pone el nombre como se va llamar el proceso/ventana.
																		* De ahi, sigue valores adicionales, de nuevo, con nosotros seria el programa que queremos que ejecute
																		* siendo los subprocesos. Si tienes el fullPath en las variables del ambiente del OS, solo pones el nombre
																		* pero conmigo, no lo hice, por lo que tuve que poner el fullPath.
																		* Al final, se pone NULL, de lo que entendi, funciona como bandera de fin de lectura.
																		* 
																		*/
			execvp("xterm", argv);										//execvp({cual es eel programa a ejecutar}, {variables adicionales});
			exit(0);													//Fin del proceso hijo
		}
		/*																//Esta comentada esta parte, porque si no, no generaria los 6 procesos de golpe, dejen esto asi
		else 															//Si es el proceso padre, esperar a que el hijo termine (ID > 0)
		{
			printf("hola\n");
			waitpid(IDprocesos, NULL, 0);								//waitpid({variable que se esta escuchando}, {el status que se desea}, {valor que se esta esperando})
		}
		*/
		i++;
	}while(i < 6);
	
	return 0;
}
