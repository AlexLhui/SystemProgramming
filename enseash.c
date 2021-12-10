#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_SIZE 255 //Max size of command

int main(int argc, char *argv[]) {
	write(1,"Welcome to ENSEA Tiny Shell !\n",strlen("Welcome to ENSEA Tiny Shell !\n"));
	write(1,"Pour quitter, tapez 'exit'.\n",strlen("Pour quitter, tapez 'exit'.\n"));
	write(1,"enseash % ",strlen("enseash % "));
	char *prompt;
	
	do {
		prompt = (char *) malloc(MAX_SIZE); //To store what the user will write
		int n = read(0,prompt,MAX_SIZE);
		prompt[n-1] = '\0';
		if (strcmp(prompt,"exit") == 0) { //strcmp equals 0 if both strings are equal
			exit(EXIT_SUCCESS); //If strings are equal, we exit the program
		}
		else {
			pid_t ret = fork(); //The command will be executed by a child
			if (ret == 0) {
				int ex = execlp(prompt,prompt,(char *)NULL); //We execute and store the value returned by exec
			}
			else if (ret > 0) { //If it the father : wait
				wait(NULL);
			}
		}
		prompt = (char *) realloc(prompt,MAX_SIZE);
		write(1,"enseash % ",strlen("enseash % "));
	} while(1);
	
	return 0;
}
