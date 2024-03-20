//TCP echo client program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 512  /* buffer size*/

int main (int argc, char* argv[ ])			//three arguments to be checked later
{
	int socket_fd;					//socket descriptor

	struct sockaddr_in servAddr;			//declare server socket address variable

	char buffer[BUFFSIZE];				//buffer holding received message echoed from the server
	char* ptr=buffer;				//pointer to move along buffer for the next byte to be recieved
	int n=0;					//number of bytes in each recv() call
	int len=0;					//length of message to be echoed in buffer
	int maxLen=BUFFSIZE;				//maximum number of bytes in buffer

	char *servIP;			//server IP address from command line
	int servPort;			//server port number from command line
	char *message;			//message specified on command line

//check for correct number of command line arguments
	if(argc !=4) {
		printf("tcp-echo-client [IP address] [Port] [Message]\n");
		exit (1);
	}

	servIP=argv[1];
	servPort=atoi(argv[2]);
	message=argv[3];

	memset (&servAddr, 0, sizeof(servAddr));		//initialize data structure
	servAddr.sin_family = AF_INET;				//this is an IPv4 address
	servAddr.sin_addr.s_addr = inet_addr(servIP);		//server IP address
	servAddr.sin_port = htons(servPort);			//server port number

//create a TCP socket stream
	if ((socket_fd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error: socket creation failed!\n");
		exit(1);
	}
	else
	printf("Socket successfully created...\n");

//connect to the server
	if ((connect (socket_fd, (struct sockaddr*)&servAddr, sizeof(servAddr))) ==-1) {
		printf("Error: connection to the server failed!\n");
		exit(1);
	}
	else
		printf("Connected to the server...\n");




//send data to the server
	send(socket_fd, message, strlen(message),0); 		//send message to server socket
	printf("Message sent out to the server Done successfully.");

//receive data from the server
//loop while receiving data
	while((n=recv(socket_fd, buffer, maxLen,0))>0);
	{
		ptr+=n;				//move pointer to next position to receive next string segment
		maxLen-=n;			//decrease maximum capacity in buffer
		len+=n;				//increase reveived number of bytes

		buffer[len]='\0';
		printf("\necho back is \t %s \n", buffer);
	}

	printf("\nPrint outside of while loop which keeps recv(): Only happens after server kills the connection first---- The echoed back message again is %s. \n", buffer);

//close client socket
	close (socket_fd);

//stop program
	return 0;



} //end main
