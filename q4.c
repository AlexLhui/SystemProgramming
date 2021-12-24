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

int enseashCommands(char *prompt, int n) { //execute the prompt command given in argument, with n the length of the prompt command
	int ex;
	int status;
	char *out; //Will be used to write in the shell
	out = (char *) malloc(MAX_SIZE);
		
		//We differientiated both exits just for the property of the shell (new line in this case)
		if (strcmp(prompt,"exit") == 0) { //strcmp equals 0 if both strings are equal
			showmsg("Bye bye ...\n");
			exit(EXIT_SUCCESS); //We exit the program
		}
		else if (n == 0) { //This second test is for a <ctrl>+d
			showmsg("Bye bye ...\n");
			exit(EXIT_SUCCESS); //We exit the program
		}
		
		else {
			pid_t ret = fork(); //We fork so that the command will be executed by the son
			if (ret == 0) {
				ex = execlp(prompt,prompt,(char *)NULL); //We execute and save the value returned by execlp
				if (ex < 0) { //If error occured during execlp
					showmsg("Error occured, please try again. \n");
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
		
		showmsg(out);
		out = (char *) realloc(out,MAX_SIZE); //Reallocation of the memory for out
		return 0;
}

int main(int argc, char *argv[]) {
	
	
	welcoming("Welcome to ENSEA Tiny Shell !\n", "Pour quitter, tapez 'exit'.\n", "enseash % \n");
	
	do {
	    char *prompt;
	    prompt = (char *) malloc(MAX_SIZE); //To store what the user will write
	    int n = read(0,prompt,MAX_SIZE);
	    prompt[n-1] = '\0'; //To erase the '\n' character made by pressing ENTER
	    enseashCommands(prompt,n);
	    prompt = (char *) realloc(prompt,MAX_SIZE); //Reallocation of the memory for prompt
	    showmsg("enseash % ");
	} while(1);
	
	return 0;
}
