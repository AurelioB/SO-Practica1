#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void loadPasswords();
char * askUser();
int validateUser();
int getNumberOfUsers();

char *argv[3];
char **passwords;
int numPasswords = 0;
int IDProceso;
int prueba = 11;

int main() {
  printf("PID: %d\n", getpid());
  loadPasswords();
  while(1) {
    if(validateUser()) {
      printf("Welcome\n");
      if(fork() == 0) {
        char *argv[] = {"./sh", (char *) 0 };
        execvp("./sh", argv);
      } else  {
        int pid, status;
        pid = wait(&status);
        if(WEXITSTATUS(status) == 123)
          exit(123);
      }
    } else
    printf("Invalid username or password. Please try again.");
  }
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
      char *newline = strchr(text, '\n'); // search for newline character
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
      if(result == 0)
        return 1;
    }
    return 0;
}