#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define	MAXLINE	128	// maximum bytes to receive and send at once

// External functions
extern int swap_wait(unsigned short port);
extern int swap_read(int sd, unsigned char buf[]);
extern void swap_close(int sd);


int main(int argc, char *argv[])
{
	unsigned short server_port;
	unsigned int server_address;
	unsigned char message[MAXLINE];
	unsigned char *outfilename;
	unsigned char outbuf[1028];
	int index=0;
	int count=0;
	int fileP;
	FILE *out_fp;
	int sd, n, out;
	int bytes_read;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	// set values for server address and port number
	server_port = htons(atoi(argv[1]));

	// connect to the port allocated to the swap server
	sd = swap_wait(server_port);
	if (sd < 0) {
		fprintf(stderr, "swap_open error\n");
		exit(0);
	}


	//when the server receives the first frame: create a new file
	if( (bytes_read = swap_read(sd,message))>0 )		//read first frame into 'message'
	{
		//transfered text is stored in message

		outfilename=message;				//first message becomes the file name
		outfilename[strlen(message)-4]='\0';		//frame has no end of string character

		out_fp = fopen(outfilename,"a");		//opn file with the naeme of the first message received
		if(out_fp==NULL)
		{
			printf("Unable to create %s \t",outfilename);
			return 1;
		}
		//new file is open
	}

	//begin-loop; server keeps receiving message until nothing is received
	while( (bytes_read=swap_read(sd,message))>0 )
	{
		if(bytes_read != 1)			//if 1:receive duplicated data, do not take the data
		{
			for(index=0; index<bytes_read-2; index++)
			{
				outbuf[count]=message[index];
				count++;
			}
						//server adds one message to outbuf
		}
	}
		//get the file data from the client until end-of-file
		//end-loop; data is in outbuf now

	//close connection to the swap server





	// close the connection to the swap servfer
	swap_close(sd);
	
	//write data to new file from outbuf
	fputs(outbuf,out_fp);
	printf(" \n\nFinal contents in outbuf sent to file is:\n%s \n",outbuf);

	//close connection to output file
	fclose(out_fp);
}
