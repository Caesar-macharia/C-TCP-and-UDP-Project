//TCP echo server program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 512		//maximum length of buffer
#define PORT 8899		//fixed server port number

int main  (void)
{
	struct sockaddr_in server_address;		//data structure for server address
	struct sockaddr_in client_address;		//data structure for client address
	int client_address_len = 0;			//length of client address
	int listen_socket;				//listen socket descriptor
	int s_socket;					//socket  descriptor

//declare buffer for server
	char buffer[BUFLEN];				//declare buffer to hold received characters for server
	char* ptr=buffer;				//pointer to point to the next bytes to receive in buffer
	int len=0;					//number of bytes to receive or send in buffer
	int maxLen=BUFLEN;				//maximum number of bytes allowed in buffer
	int n=0;					//number of bytes received for each recv() call

//define how many client's request can be in the waiting queue
	int waitSize=16;				//size of waiting clients

	memset(&server_address, 0, sizeof (server_address));		//initialize server address data structure
	server_address.sin_family = AF_INET;				//populate family field - IPv4 protocol
	server_address.sin_port = htons(PORT);				//set port number
	server_address.sin_addr.s_addr = htons(INADDR_ANY);		//set Ip address to IPv4 value localhost

//create a TCP socket; return -1 on failure
	if ( (listen_socket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP) ) == -1)
	{
		printf("Error: Listen socket failed!\n");
		exit(1);
	}

//bind the socket to the server address; return -1 on failure
	if ((bind(listen_socket, (struct sockaddr *)&server_address, sizeof (server_address))) == -1)
	{
		printf("Error: binding failed!\n");
		exit(1);
	}

	printf("Server is listening, waiting to connect...\n");


//listen for connections
	if(listen(listen_socket,waitSize) == -1)
	{
		printf("Error: listening failed!");
		exit(1);
	}


	while(1)
	{
//accept conncetions from client
		if((s_socket = accept(listen_socket,(struct sockaddr*)&client_address,(socklen_t*)&client_address_len))==-1)
		{
			printf("Error: accepting failed! Can't open a socket to accept data transmit.");
			exit(1);
		}

		printf("\n\tlisten_socket accepts a client request return a socket for data trasnmit.");
		printf("\tclient connected with client's IP address at %s\n",inet_ntoa(client_address.sin_addr));

/*keep receiving as long as the connection by the client is maintained*/
		while((n=recv(s_socket,ptr,maxLen,0))>0)
		{
			printf("Received message segment from client is %s. \n",buffer);
			ptr+=n;		//move pointer to next position for bytes to be received along buffer
			maxLen-=n;	//decrease maximum bytes to be received
			len+=n;		//increase received bytes

			send(s_socket,buffer,len,0);
		}
		printf("\n\nFinal: outside of recv() working zone: Buffer content to be sent out is %s:\n", buffer);

		close(s_socket);
	}

	close(listen_socket);		//close descriptor referencing server socket
	return 0;

}	//end main
