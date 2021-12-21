#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
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

int redirection(char *arglist[MAX_SIZE],int i) {
	char *filename = arglist[i-1];
	int fd = creat(filename, S_IRUSR | S_IWUSR | S_IRGRP);
	char *newarglist[MAX_SIZE];
	int j;
	for (j = 0; j < i-3; j++) { //We recreate the argument list without the filename, and the '<' or '>' character
		newarglist[j] = arglist[j];
	}
	//newarglist[j+1] = arglist[i-1]; //filename
	newarglist[j+1] = (char *)NULL;
	write(fd,execvp(newarglist[0],newarglist));
	return 0;
}

int main(int argc, char *argv[]) {
	char *prompt; //To store what the user will write
	int ex;
	int status;
	char *out; //Will be used to write in the shell
	struct timespec start, stop;
	double elapsed_time;
	const char delim[1] = " "; //Used to cut the input command and make the list of arguments
	char *token;
	char *arglist[MAX_SIZE];
	bool redirect = false;
	
	write(1,"Welcome to ENSEA Tiny Shell !\n",strlen("Welcome to ENSEA Tiny Shell !\n"));
	write(1,"Pour quitter, tapez 'exit'.\n",strlen("Pour quitter, tapez 'exit'.\n"));
	write(1,"enseash % ",strlen("enseash % ")); //For first execution
	
	do {
		prompt = (char *) malloc(MAX_SIZE);
		out = (char *) malloc(MAX_SIZE);
		token = (char *) malloc(MAX_SIZE);
		//redirect = (char *) malloc(3); //Size enough to store 'in' or 'out' strings (2 or 3 characters)
		int n = read(0,prompt,MAX_SIZE);
		prompt[n-1] = '\0'; //To erase the '\n' character made by pressing ENTER
		
		//We differientiated both exits just for the property of the shell (new line in this case)
		if (strcmp(prompt,"exit") == 0) { //strcmp equals 0 if both strings are equal
			write(1,"Bye bye ...\n",strlen("Bye bye ...\n"));
			exit(EXIT_SUCCESS); //We exit the program
		}
		else if (n == 0) { //This second test is for a <ctrl>+d
			write(1,"\nBye Bye ...\n",strlen("\nBye bye ...\n"));
			exit(EXIT_SUCCESS);
		}
		
		else {
			int i = 0;
			token=strtok(prompt,delim);
			while (token != NULL) {
				if (strcmp(token,"<") == 0) {
					redirect = true;
				}
				else if (strcmp(token,">") == 0) {
					redirect = true;
				}
				arglist[i]=token;
				token=strtok(NULL,delim);
				i++;
			}
			arglist[i] = (char *)NULL; //The last argument must be a NULL pointer
			
			pid_t ret = fork(); //We fork so that the command will be executed by the son
			if (ret == 0) {
				if (!redirect) {
					ex = execvp(arglist[0],arglist); //We execute and save the value returned by execlp
				}
				else if (redirect) {
					ex = redirection(arglist,i);
				}
				if (ex < 0) { //If error occured during execlp
						write(1,"Error occured, please try again. \n", strlen("Error occured, please try again. \n"));
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
		redirect = false;
	
	} while(1);
	
	return 0;
}


