//author Griffin McCool
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <unistd.h>

//print command line usage if used incorrectly
void printUsage(){
	printf("Usage: osmsg -s [recipient] [\"message\"] \nOR: osmsg -r\n");
}

//send message
int send_msg(char *to, char *msg, char *from){
	//send message syscall
	return syscall(441, to, msg, from);
}

//get message
int get_msg(char *to, char *msg, char *from){
	//get message syscall
	return syscall(442, to, msg, from);
}

int main(int argc, char *argv[]){
	char *msg;
	char *to;
	char *from;
	int ret = 1;
	//determine whether to send, receive, or print usage
	if (argc == 2){
		if (!strcmp(argv[1], "-r")){
			while (ret == 1){
				//allocate space for strings
				//I'm also assuming that the longest incoming message
				//would be 1000 characters, which is a reasonable amount
				//for a message. Along with this, the longest incoming
				//username would be at max 50 characters.
				msg = malloc(1000);
				from = malloc(50);
				//get messages & print
				ret = get_msg(getenv("USER"), msg, from);
				// if return value from syscall is 0 or 1, print the message
				if (ret >= 0) printf("%s said: \"%s\"\n", from, msg);
				else printf("No messages found.\n"); // if return value < 0 there are no messages
				//free allocated space
				free(msg);
				free(from);
			}
		} else printUsage(); // print usage if used incorrectly
	} else if (argc == 4){
		if (!strcmp(argv[1], "-s")){
			if (strlen(argv[3]) > 1000){
				printf("Messages cannot be more than 1000 characters.\n");
			} else if (strlen(argv[2]) > 50){
				printf("Username cannot be more than 50 characters.\n");
			} else {
				//allocate space for strings
				msg = malloc(strlen(argv[3]) + 3);
				to = malloc(strlen(argv[2]) + 1);
				//copy message and recipient from command line
				strcpy(msg, argv[3]);
				strcpy(to, argv[2]);
				//send message
				ret = send_msg(to, msg, getenv("USER"));
				//tell user that message was sent
				if (ret == 0) printf("Message sent.\n");
				//if ret < 0, message couldn't send, thus we print an error
				if (ret < 0) printf("Message failed to send.\n");
				//free allocated space
				free(msg);
				free(to);
			}
		} else printUsage(); // print usage if used incorrectly
	} else {
		printUsage();
	}
	return 0;
}
