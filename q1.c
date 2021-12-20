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
	write(1,"enseash % \n",strlen("enseash % \n"));
	
	return 0;
}
