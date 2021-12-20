#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define MAX_SIZE 255 //Max size of command

int main(int argc, char *argv[]) {
	write(1,"Welcome to ENSEA Tiny Shell !\n",strlen("Welcome to ENSEA Tiny Shell !\n"));
	write(1,"Pour quitter, tapez 'exit'.\n",strlen("Pour quitter, tapez 'exit'.\n"));
	write(1,"enseash % ",strlen("enseash % ")); //For first execution
	char *prompt;
	//char *out;
	int ex;
	
	do {
		prompt = (char *) malloc(MAX_SIZE); //To store what the user will write
		int n = read(0,prompt,MAX_SIZE);
		prompt[n-1] = '\0';
		if (strcmp(prompt,"exit") == 0 || n == 0) { //strcmp equals 0 if both strings are equal
			write(1,"\nBye bye ...\n",strlen("\nBye bye ...\n"));
			exit(EXIT_SUCCESS); //If strings are equal, we exit the program
		}
		else {
			pid_t ret = fork(); //The command will be executed by a child
			if (ret == 0) {
				ex = execlp(prompt,prompt,(char *)NULL); //We execute and store the value returned by exec
				if (ex < 0) { //If error occured during execlp
					write(1,"Commande non trouvée \n", strlen("Commande non trouvée \n"));
				}
			}
			else if (ret > 0) { //If it the father : wait
				wait(NULL);
			}
		}
		prompt = (char *) realloc(prompt,MAX_SIZE);
		//out = strncat(out,sprintf(ex,1),MAX_SIZE);
		//out = "\nBye bye ...\n";
		//write(1,out,strlen(out));
		//out = (char *) realloc(out,MAX_SIZE);
	} while(1);
	
	return 0;
}
