#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int newTerm();

int main()
{
	
	int i = 0;	
	int pid = -1;	
	int pids[6];
	int status = -1;
	int killall = 0;

	for(i = 0; i < 6; i++) {
		pids[i] = newTerm();
		printf("Process PID %d started\n", pids[i]);
	}
	
	while (i > 0) {
		pid = wait(&status);
		status = readStatus("status");
		printf("PID %d finished execution with status %d\n", pid, status);

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
			killall = 1;
			for(i = 0; i < 6; i++) {
				kill(pids[i], SIGUSR1);
			}
		}
		--i; 
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
			return (int) strtol(line, (char **)NULL, 10);
		}
	}
	return -1;
}
