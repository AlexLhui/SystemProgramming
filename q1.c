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

int welcoming(char *welcsentence, char *exitsentence, char *shellname) { //This function print the welcoming messages
	showmsg(welcsentence);
	showmsg(exitsentence);
	showmsg(shellname);	
	return 0;
}

int main(int argc, char *argv[]) {
	
	welcoming("Welcome to ENSEA Tiny Shell !\n", "Pour quitter, tapez 'exit'.\n", "enseash % \n");
	
	return 0;
}
