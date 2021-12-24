#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#define MAX_SIZE 255 //Arbitrary max size of command
#define MAX_ARGS 10 //Arbitrary max number of arguments for a function call
#define MILLION 1000000

double calculate_time(struct timespec start, struct timespec stop) {
	double elapsed_time;
	elapsed_time =((stop.tv_nsec-start.tv_nsec)/MILLION);
	if (elapsed_time<0) { //If the time is negative, we add 1sec
		elapsed_time += 1000;
	}
	return elapsed_time;
}

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
	struct timespec start, stop;
	double elapsed_time;
	const char delim[1] = " "; //Used to cut the input command and make the list of arguments
	char *token;
	char *arglist[MAX_SIZE];
	out = (char *) malloc(MAX_SIZE);
	prompt[n-1] = '\0'; //To erase the '\n' character made by pressing ENTER
		
	out = (char *) malloc(MAX_SIZE);
	token = (char *) malloc(MAX_SIZE);
		
	//We differientiated both exits just for the property of the shell (new line in this case)
	if (strcmp(prompt,"exit") == 0) { //strcmp equals 0 if both strings are equal
		showmsg("Bye bye ...\n");
		exit(EXIT_SUCCESS); //We exit the program
	}
	else if (n == 0) { //This second test is for a <ctrl>+d
		showmsg("Bye bye ...\n");
		exit(EXIT_SUCCESS);
	}
		
	else {
		int i = 0;
		token=strtok(prompt,delim);
		while (token != NULL) {
			arglist[i]=token;
			token=strtok(NULL,delim);
			i++;
		}
		arglist[i] = (char *)NULL;
			
		pid_t ret = fork(); //We fork so that the command will be executed by the son
		if (ret == 0) {
			ex = execvp(arglist[0],arglist); //We execute and save the value returned by execlp
			if (ex < 0) { //If error occured during execlp
				showmsg("Error occured, please try again. \n");
				exit(EXIT_FAILURE);
			}
		}
		else if (ret > 0) { //If it is the father, he will wait
			clock_gettime(CLOCK_MONOTONIC, &start);
			wait(&status);
			clock_gettime(CLOCK_MONOTONIC, &stop);
			elapsed_time = calculate_time(start,stop);
		}
	}
		
	//Now we check the return code of the previously executed command
	if (WIFEXITED(status)) {
		sprintf(out,"enseash [exit:%d|%.0fms] %% ",WEXITSTATUS(status),elapsed_time);
	}
	else if (WIFSIGNALED(status)) {
		sprintf(out,"enseash [sign:%d|%.0fms] %% ",WTERMSIG(status),elapsed_time);
	}
		
	write(1,out,strlen(out));
	free(prompt);
	free(out);
	free(token);
	out = (char *) realloc(out,MAX_SIZE); //Reallocation of the memory for out
	return 0;
}

int main(int argc, char *argv[]) {
	
	welcoming("Welcome to ENSEA Tiny Shell !\n", "Pour quitter, tapez 'exit'.\n", "enseash % \n");
	
	do {
		char *prompt; //To store what the user will write
		prompt = (char *) malloc(MAX_SIZE); //To store what the user will write
		int n = read(0,prompt,MAX_SIZE);
		prompt[n-1] = '\0'; //To erase the '\n' character made by pressing ENTER
		enseashCommands(prompt,n);
		prompt = (char *) realloc(prompt,MAX_SIZE); //Reallocation of the memory for prompt
		showmsg("enseash % ");
	} while(1);
	
	return 0;
}

