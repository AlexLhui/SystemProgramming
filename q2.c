#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define MAX_SIZE 255 //Max size of command

int main(int argc, char *argv[]) {
	char *prompt;
	int ex;
	int status;
	
	write(1,"Welcome to ENSEA Tiny Shell !\n",strlen("Welcome to ENSEA Tiny Shell !\n"));
	write(1,"Pour quitter, tapez 'exit'.\n",strlen("Pour quitter, tapez 'exit'.\n"));
	write(1,"enseash % ",strlen("enseash % ")); //For first execution
	
	do {
		prompt = (char *) malloc(MAX_SIZE); //To store what the user will write
		int n = read(0,prompt,MAX_SIZE);
		prompt[n-1] = '\0'; //To erase the '\n' character made by pressing ENTER
		pid_t ret = fork(); //We fork so that the command will be executed by the son
		if (ret == 0) {
			ex = execlp(prompt,prompt,(char *)NULL); //We execute and save the value returned by execlp
			if (ex < 0) { //If error occured during execlp
				write(1,"Error occured, please try again. \n", strlen("Error occured, please try again. \n"));
				exit(EXIT_FAILURE);
			}
		}
		else if (ret > 0) { //If it is the father, he will wait
			wait(&status);
		}
		prompt = (char *) realloc(prompt,MAX_SIZE); //Reallocation of the memory for prompt
		write(1,"enseash % ",strlen("enseash % "));
	} while(1);
	
	return 0;
}
