# C-TCP-and-UDP-Project

Part 1
  C programming Language and Sockets
  UDP Time Client
  •	The user will specify the IP address and port 9988 as command-line parameters.
  •	The client program will create a socket using the IP address and port number specified on the command line.
  •	The client will send a time request to the server using the sendto() function.
  •	The client will receive the current time from the server via the recvfrom() function.
  •	The client will exit.
  UDP Time Server
  •	The server program will create a socket using the default IP address and port 8899.
  •	The server will bind the socket to port 9988.
  •	The server will receive a time request from the client via the recvfrom() function.
  •	The server will send the current time to the client using the sendto() function.
  •	The server will exit.
  
    You will need to test your programs on two terminals, one terminal for the server and another terminal for the client. The port number and IP address for the client should be specified as command line arguments.
  
    Compile and run the client and server programs
    1.	# Compile the UDP server program
    gcc udp-time-server.c -o server
    2.	# Run the program server program from the command line
    ./server
    3.	# Compile the UDP client program
    gcc udp-time-client.c -o client
    4.	# Run the client program from the command line; specify the IP address and port number
    ./client 127.0.0.1 9988
    
Part 2
  Basic TCPIP Socket Programming
  TCP Echo Server Implementation
  •	In the TCP echo server, we create a socket and bind it to a port number.
  •	After binding, the server waits and listens for incoming connections.
  •	When received, the server accepts a connection from the client machine.
  •	The server receives data from the client using recv() function and echoes the same data back using the send() function.
  TCP Echo Client Implementation
  •	In the TCP echo client, we create a socket.
  •	We specify the client address, the port number, and the message to be sent using values passed from the command line.
  •	We send messages to the server using the send() function and receive data from the server using the recv() function.
  
    You will need to test your programs on two terminals, one terminal for the server and another terminal for the client. The port number and IP address for the client should be specified as command line arguments.
  
    1.	# Compile the TCP echo server program 
    $ gcc tcp_echo_server.c -o tcp_echo_server
    2.	# Run the TCP Echo server program from the command line
    ./tcp_echo_server
    3.	# Compile the TCP echo client program
    $ gcc tcp_echo_client.c -o tcp_echo_client
    4.	# Run the client program from the command line; specify the IP address, port number, and message
    ./tcp_echo_client 127.0.0.1 8899 message
    
Part 3
  Programming at the Data Link Layer
  SWAP Client Implementation
  In swap_client.c, there are three functions with some global variables:
  •	int swap_open(unsigned int addr, unsigned short port);
  •	int swap_write(int sd, char *buf, int length); // you need to design and implement
  •	void swap_close(int sd);
  You may use the global variable S for the control variable on the SWAP client in swap_client.c.
  SWAP Server Implementation
  In swap_server.c, there are three functions with some global variables:
  •	int swap_wait(unsigned short port);
  •	int swap_read(int sd, char *buf); // you need to design and implement
  •	void swap_close(int sd);
  
    You will need to test your programs on two terminals, one terminal for the server and another terminal for the client. The port number and IP address for the client should be specified as command line arguments.
    1.	# Compile the SWAP server program $
    gcc test_swap_server.c swap_server.c sdp.c checksum.c -o test_swap_server
    2.	# Run the SWAP server program from the command line; specify the port number
    ./test_swap_server 8899
    3.	# Compile the SWAP client program
    $ gcc test_swap_client.c swap_client.c sdp.c checksum.c -o test_swap_client
    4.	# Run the client program from the command line; specify the IP address and port number
    ./test_swap_client 127.0.0.1 8899
  
  If there are no errors in your implementation of SWAP, then swap_server will print the message sent from swap_client.

Part 4
  A basic file transfer apllication
  You will need to test your programs on two terminals, one terminal for the server and another terminal for the client. The port number and IP address for the client should be specified as command line arguments, as well as the names of the input and output file. If the output file does not exist, your program should create the output file.

    1.	# Compile the FTA server program
    $ gcc fta_server.c swap_server.c sdp.c checksum.c -o fta_server
    2.	# Run the SWAP server program from the command line; specify the port number on the command line
    ./fta_server 8899
    3.	# Compile the SWAP client program
    $ gcc fta_client.c swap_client.c sdp.c checksum.c -o fta_client
    4.	# Run the client program from the command line; specify the IP address, port number and input-filename.
    ./fta_client 127.0.0.1 8899 input-filename
      
        Note
        input-filename is a text file that you create to send to the server ; a simple text file with 5 lines is sufficient.
    
    5.	If there are no errors in your implementation of FTA, then fta_client will transfer the input file to fta_server. The fta_server will write the contents of the file sent by fta_client to the output file. For the sake of simplicity, you may hardcode the name of the output file in fta_server.c
    6.	After you copy a file using your FTA application, compare the input file to the output file o see if the destination file was copied correctly. Use the diff command to compare the two files.


