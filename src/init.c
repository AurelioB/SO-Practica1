#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int newTerm();

int main()
{
	//Declaración de variables
	int i = 0;	//Contador para la generacion de los 6 procesos
	int pid = -1;	//La variable que tendra el ID de los procesos que se van creando
	int pids[6];
	int status = -1;
	int killall = 0;
	/*
	* Ciclo encargado de crear los sub-procesos
	* */
	for(i = 0; i < 6; i++) {
		pids[i] = newTerm();
		printf("Process PID%d started\n", pids[i]);
	}
	
	while (i > 0) {
		pid = wait(&status);
		printf("PID %d exited with status %x. %d \n", pid, status, WEXITSTATUS(status));

		status = readStatus("status");
		
		if(status == 0 && !killall) {
  
			int n;
			for(n = 0; n < 6; n++) {
				if(pids[n] == pid) break;
			}
			printf("Restarting closed process #%d\n", n);
			while( (pid = newTerm()) == -1);
			pids[n] = pid;
			
			++i;
		} else if(status == 123) {
			printf("Received shutdown signal from PID: %d\n", pid);
			//kill(pid);
			killall = 1;
			for(i = 0; i < 6; i++) {
				kill(pids[i], SIGUSR1);
			}
		}
		--i; // TODO(pts): Remove pid from the pids array.
	}

	return 0;
}


int newTerm() {
	int pid = fork();
	if(pid == -1) {
		printf("Fork error: %d\n", pid);
		exit(EXIT_FAILURE);
	}
	else if(pid == 0) {
		char *argv[] = {"xterm", "-e", "echo 0 > status; ./getty; echo $? > status", (char *) 0 };
		execvp("xterm", argv);
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
