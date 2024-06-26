/*
*	swap_client.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define	MAXLINE 128	// maximum characters to receive and send at once
#define	MAXFRAME 256

extern int swap_connect(unsigned int addr, unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);
extern unsigned short checksum(unsigned char buf[], int length);

int session_id = 0;
int s = 0;	// frame number sent

int swap_open(unsigned int addr, unsigned short port)
{
	int	sockfd;		// sockect descriptor
	struct	sockaddr_in	servaddr;	// server address
	char	buf[MAXLINE];
	int	len, n;

	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	connect to a server
	*/

	session_id = swap_connect(addr, port);	// in sdp.o

	/*
	*	return the session id
	*/

	return session_id;
}

int swap_write(int sd, unsigned char *buf, int length)
{
	int nRcv;
	int numSent;
	int i;
	int buflen;
	int acklen;
	unsigned char frame[MAXFRAME];
	unsigned char checkSend;
	unsigned char ack[MAXFRAME];
	unsigned char checkAck;

	if (session_id == 0 || sd != session_id)		//if not connect() successfully or sd not be next session_id
		return -1;

	/*build a DATA frame: message in buf is to be sent to remote socket, plus frame# */
	frame[0] = s+'0';				//frame# is added as the first element of frame array

	for(i=0;i<length;i++)			//length==10: the other characters in frame are data message from network layer
	{
		frame[i+1]=buf[i];		//only add char in buff[0]-buff[9] to f[1]-f[10]; f[0] is for frame#
	}


		//compute checksum value for f[0]-f[10]:(including head s#):length+1=11
	checkSend = checksum(frame,length+1);
		//add checksum value to the end of the frame. f#+data:f[0]-f[10], checksum:f[11]
	frame[length+1]=checkSend;		//length==10:f[11]=checksum;

	/*send frame out to the server*/
	while(1){
		numSent=sdp_send(sd, frame,length+2);			//n=#char sent=length+2;extra head:s; tail:checksum

		s++;				//next frame sequence#, if good:ACK with next 's' is received within timeout

	/* read ACK frame with a timer:
	 * exepcting ACK with the next frame sequence#:s. if received ACK within timeout, send next frame with frame 
	 * sequence#:s. If not receive ACK within timeout, resend last frame. */

		nRcv= sdp_receive_with_timer(sd,ack,2000);


		if(nRcv>0){			//receive ACK within timeout, no other errors
	/*compute received ack's chesksum, if wrong, drop it and send last frame*/

			checkAck=checksum(ack,4);		//checksum only computes first 4 char

	//if checksum matches sender's checksum and ACK has right next frame#--> send another frame by calling this function again
			if(checkAck==ack[4] && ack[0]==(s+'0'))		//ack pattern is f#, 3 char, checksum
			{

				return numSent;		//return the length sent and stop loop
			}

			if(checkAck==ack[4] && ack[0]==(s-1+'0'))	//if receive duplicate ACK
			{

				return 1;			//return 1; terminate this function, start new write again
			}

				//frame# or chesksum wrong: back to loop to send frame again.
		}		//reduce frame# s because it will increase in the start loop again

		s--;		//not receive any ACK frame, send this frame again.
//		printf("\n Timeout fail to receive ACK! Resend the frame again");
	}			//any other errors by sdp_receive(): resend last frame again from while loop again



}

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
