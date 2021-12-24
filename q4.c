#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define MAX_SIZE 255 //Max size of command

int showmsg(char *msg) { 
	int len = strlen(msg);
	write(1, msg, len);
	return 0;
}

int welcoming(char *welcsentence, char *exitsentence, char *shellname) { 
	showmsg(welcsentence);
	showmsg(exitsentence);
	showmsg(shellname);	
	return 0;
}

int readAndExecv3(int maxSizeOfCommand) {
	char *prompt; //To store what the user will write
	int ex;
	int status;
	char *out; //Will be used to write in the shell
	prompt = (char *) malloc(maxSizeOfCommand);
		out = (char *) malloc(maxSizeOfCommand);
		int n = read(0,prompt,maxSizeOfCommand);
		prompt[n-1] = '\0'; //To erase the '\n' character made by pressing ENTER
		
		//We differientiated both exits just for the property of the shell (new line in this case)
		if (strcmp(prompt,"exit") == 0) { //strcmp equals 0 if both strings are equal
			write(1,"Bye bye ...\n",strlen("Bye bye ...\n"));
			exit(EXIT_SUCCESS); //We exit the program
		}
		else if (n == 0) { //This second test is for a <ctrl>+d
			write(1,"\nBye bye ...\n",strlen("\nBye bye ...\n"));
			exit(EXIT_SUCCESS); //We exit the program
		}
		
		else {
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
		}
		
		//Now we check the return code of the previously executed command
		if (WIFEXITED(status)) {
			sprintf(out,"enseash [exit:%d] %% ",WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status)) {
			sprintf(out,"enseash [sign:%d] %% ",WTERMSIG(status));
		}
		
		write(1,out,strlen(out));
		prompt = (char *) realloc(prompt,maxSizeOfCommand); //Reallocation of the memory for prompt
		out = (char *) realloc(out,maxSizeOfCommand); //Reallocation of the memory for out
		return 0;
}

int main(int argc, char *argv[]) {
	
	
	welcoming("Welcome to ENSEA Tiny Shell !\n", "Pour quitter, tapez 'exit'.\n", "enseash % \n");
	
	do {
		readAndExecv3(MAX_SIZE);
	} while(1);
	
	return 0;
}
