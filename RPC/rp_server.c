#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

#include <errno.h>
#include <error.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "rp_calls.h"


/*************************************************************************/
//                       DO NOT REMOVE THIS FUNCTION!!                   //
/*************************************************************************/

void this_is_for_the_autograder_do_not_delete(unsigned short port) {
    
    int fd;
    if((fd = open(".server_port", O_CREAT | O_TRUNC | O_WRONLY, 0600)) == -1)
        perror("autograder port file open error");

    char buf[7];
    sprintf(buf, "%d%c", port, '\0');
    if(write(fd, buf, strlen(buf)) == -1)
       perror("autograder port file write error"); 

    close(fd);
    printf("SERVER LISTENING ON PORT %d\n", port);

}


/*************************************************************************/
//                                                                       //
//     read_from_client(): reads next packet of data                     //
//                         and returns pointer to the                    //
//                         data read                                     //
//                                                                       //
//     Calling Convention:                                               //
//                                                                       //
//              1. Reading an integer:                                   //
//                                                                       //
//                      int var = *(int*) read_from_client();            //
//                                                                       //
//              2. Reading a string:                                     //
//                                                                       //
//                      char *string = read_from_client();               //
//                                                                       //
//     How it works:                                                     //
//                                                                       //
//              The communication for this assignment is                 //
//              abstracted to the following:                             //
//                                                                       //
//                      Any data sent across the network                 //
//                      is preceeded by a fixed size packet              //
//                      indicating the size of the data                  //
//                      contained in the subsequent packet               //
//                                                                       //
//              Ex 1:                                                    //
//                      Sending an integer is done like so:              //
//                                                                       //
//                      server --> [size of int] --> client              //
//                      server --> [integer value] --> client            //
//                                                                       //
//              Ex 2:                                                    //
//                                                                       //
//                      Sending a string is done like so:                //
//                                                                       //
//                      server --> [length of string] --> client         //
//                      server --> [string data] --> client              //
//                                                                       //
/*************************************************************************/

void* read_from_client(){
    size_t size;
    char *buf;
    int rtrn; 

    if((rtrn = read(0, &size, sizeof(size))) < 1) {
        if(rtrn == -1) {
            perror("read_from_client read 1");
            exit(-1);
        }
        return NULL;
    }

    size = ntohl(size);
    
    buf = malloc(size);
    if((rtrn = read(0, buf, size)) < 1) {
        if(rtrn == -1) {
            perror("read_from_client read 2");
            exit(-1);
        }
        return NULL;
    }
    return buf;
}

/*************************************************************************/
//                                                                       //
//     send_to_client(): sends data to the client by sending             //
//                       a packet containing the size of the             //
//                       data being sent prior to the data itself        //
//                                                                       //
//     Calling Convention:                                               //
//                                                                       //
//              1. Sending an integer:                                   //
//                                                                       //
//                      int my_int = 5;                                  //
//                      send_to_client(&my_int, sizeof(my_int);          //
//                                                                       //
//              2. Sending a string:                                     //
//                                                                       //
//                      char string[] = "some string";                   //
//                      send_to_client(string, strlen(string));          //
//                                                                       //
/*************************************************************************/

int send_to_client(void *data, size_t size){
   
    size_t nb_size = htonl(size);
    
    if(write(1, &nb_size, sizeof(size)) == -1)
        return -1;

    return write(1, data, size);

}



/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
//                                                                       //
//             YOU MUST IMPLEMENT THE FOLLOWING FUNCTIONS                //
//                                                                       //
//                  |               |               |                    //
//                  |               |               |                    //
//                  |               |               |                    //
//                \ | /           \ | /           \ | /                  //
//                 \|/             \|/             \|/                   //
//                  V               V               V                    //
//                                                                       //
//                                                                       //
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


/*************************/
//       CHECKSUM        //
/*************************/

int checksum(int fd) {
    
  // Seek to beginning of file
  lseek(fd, 0, SEEK_SET);
  char buf[4096];
  unsigned char checksum = 0;
  int num;
  num = read(fd, buf, 4096);
  // Read blocks of size 4096 from file
  // Parse all 4096 bytes in block and perform checksum
  while(num > 0) {
    for(int i = 0; i < num; i++) {
      checksum ^= buf[i];
    }

    num = read(fd, buf, 4096);
  }

  
  // Return the checksum computed
  return checksum;
}


/*************************/
//      HANDLE OPEN      //
/*************************/

void handle_open() {
       
  // Read in the argumets of sent by client 
  char *pathname;
  int *flags;
  mode_t *mode;
  
  int fd;
  
  pathname = read_from_client();
  flags = (int *) read_from_client();
  // Call the syscall with arguments
  if((O_CREAT & flags) > 0) {
    mode = (mode_t *) read_from_client();
    fd = open(pathname, *flags, *mode);
    free(mode);
  }

  else {
    fd = open(pathname, *flags);
  }
  // Send the return of open to the client
  send_to_client(fd, sizeof(int));
  
  // Send the errno information to the client
  send_to_client(errno, sizeof(errno));

  free(pathname);
  free(flags);
} 


/*************************/
//      HANDLE CLOSE     //
/*************************/

void handle_close() {
        
  // Read in the argumets of sent by client 
  int* fd = (int *) read_from_client();
  
  // Call the syscall with arguments
  int err = close(*fd);
  
  // Send the return of open to the client
  send_to_client(err, sizeof(int));
  
  // Send the errno information to the client
  send_to_client(errno, sizeof(int));
  
  free(fd);
} 


/*************************/
//      HANDLE READ      //
/*************************/

void handle_read() {
  
  // Read in the argumets of sent by client 
  int *fd = (int *) read_from_client();
  char *buf = read_from_client();
  size_t *count = (size_t *) read_from_client();
  int num;
  
  // Call the syscall with arguments
  num = read(*fd, buf, *count);
  
  send_to_client(num, sizeof(int));
  send_to_client(errno, sizeof(int));

  free(fd);
  free(buf);
  free(count);
} 


/*************************/
//      HANDLE WRITE     //
/*************************/

void handle_write() {
  
  // Read in the argumets of sent by client 
  int *fd = (int *) read_from_client();
  char *buf = read_from_client();
  size_t *count = (size_t *) read_from_client();
  int num;

  // Call the syscall with arguments
  num = write(*fd, buf, *count);
  
  // Send the return of open to the client
  send_to_client(num, sizeof(int));
  
  // Send the errno information to the client
  send_to_client(errno, sizeof(int));
  
} 


/*************************/
//      HANDLE LSEEK     //
/*************************/

void handle_lseek() {
  
  // Read in the argumets of sent by client 
  int *fd = (int *) read_from_client();
  off_t *offset = (off_t *) read_from_client();
  int *whence = (int *) read_from_client();
  off_t num;
  
  // Call the syscall with arguments
  num = lseek(*fd, *offset, *whence);
  
  // Send the return of open to the client
  send_to_client(num, sizeof(off_t));
  
  // Send the errno information to the client
  send_to_client(errno, sizeof(int));
  
} 


/*************************/
//    HANDLE CHECKSUM    //
/*************************/

void handle_checksum() {
  
  // Read in the argumets of sent by client 
  int *fd = (int *) read_from_client();
  // Call the syscall with arguments
  
  // Send the return of open to the client
  
  // Send the errno information to the client
  
}


/*************************/
//          MAIN         //
/*************************/

int main() {

    struct sockaddr_in client_addr, server_addr;

    // Zero out server and client addr structs
    bzero((char*) &client_addr, sizeof(client_addr));
    bzero((char*) &server_addr, sizeof(server_addr));
    
    // Get socket file descriptor
    const int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
   
    // Load server address information (family, sin_addr.s_addr, sin_port)
    server_addr.sin_family = (short) AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADOR_ANY);
    server_addr.sin_port = htons(0);
    
    // Bind server address to socket
    bind(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    
    // These lines will print out the port number used and write
    // it to a file that will be used by the grader
    //                          
    //                          DO NOT DELETE!! 
    //                          
    socklen_t s_len = sizeof(server_addr);
    getsockname(sock_fd,(struct sockaddr *) &server_addr, &s_len);
    this_is_for_the_autograder_do_not_delete(ntohs(server_addr.sin_port));

    // Set socket to listen for connections
    //The backlog number can be set higher for more connections
    listen(sock_fd, 1);
    
    /************************************/
    //         PART 2 OF PROJECT        //                                    
    /************************************/
        
        // Wait for client to connect
        const int conn = 0;
        
        // Redirects stdin and stdout to the socket
        // These lines must be here to work with the send_to_client and read_from_client functions
        dup2(conn, 0);
        dup2(conn, 1);
        close(conn); 

	socklen_t s_len2 = sizeof(client_addr);
	conn = accept(sock_fd, (struct sockaddr*) &client_addr, (socklen_t*) &s_len2);
        // Read the type sent by the client
	char *string = read_from_client();
	
        // Do the appropriate action for the type sent by calling the appropriate handler
	while(string != NULL) {
	  if(strcmp(string, "open") == 0) {
	    handle_open();
	  }
	  
	  else if(strcmp(string, "read") == 0) {
	    handle_read();
	  }
	  
	  else if(strcmp(string, "write") == 0) {
	    handle_write();
	  }
	  
	  else if(strcmp(string, "close") == 0) {
	    handle_close();
	  }
	  
	  else if(strcmp(string, "lseek") == 0) {
	    handle_lseek();
	  }
	  
	  else if(strcmp(string, "checksum") == 0) {
	    handle_checksum();
	  }

	  free(string);
	  string = read_from_client();
	}
        // Wait for more commands (maybe a loop)
	
	
        // Exit when EOF is sent to server (i.e. child has closed the connection)

	
    /************************************/
    //        PART 2.5 OF PROJECT       //                                    
    /************************************/
    
        // Modify the Part 2 code to now fork for every incoming connection

    return 0;        
}
