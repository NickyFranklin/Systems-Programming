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


static int sock_fd;

/*************************************************************************/
//                                                                       //
//     read_from_server(): reads next packet of data                     //
//                         and returns pointer to the                    //
//                         data read                                     //
//                                                                       //
//     Calling Convention:                                               //
//                                                                       //
//              1. Reading an integer:                                   //
//                                                                       //
//                      int var = *(int*) read_from_server();            //
//                                                                       //
//              2. Reading a string:                                     //
//                                                                       //
//                      char *string = read_from_server();               //
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
//                      client --> [size of int] --> server              //
//                      client --> [integer value] --> server            //
//                                                                       //
//              Ex 2:                                                    //
//                                                                       //
//                      Sending a string is done like so:                //
//                                                                       //
//                      client --> [length of string] --> server         //
//                      client --> [string data] --> server              //
//                                                                       //
/*************************************************************************/

void* read_from_server(){
    size_t size;
    char *buf;
    int rtrn; 

    if((rtrn = read(sock_fd, &size, sizeof(size))) < 1) {
        if(rtrn == -1) {
            perror("read_from_server read 1");
            exit(-1);
        }
        return NULL;
    }

    size = ntohl(size);
    
    buf = malloc(size);
    if((rtrn = read(sock_fd, buf, size)) < 1) {
        if(rtrn == -1) {
            perror("read_from_server read 2");
            exit(-1);
        }
        return NULL;
    }
    return buf;
}

/*************************************************************************/
//                                                                       //
//     send_to_server(): sends data to the server by sending             //
//                       a packet containing the size of the             //
//                       data being sent prior to the data itself        //
//                                                                       //
//     Calling Convention:                                               //
//                                                                       //
//              1. Sending an integer:                                   //
//                                                                       //
//                      int my_int = 5;                                  //
//                      send_to_server(&my_int, sizeof(my_int);          //
//                                                                       //
//              2. Sending a string:                                     //
//                                                                       //
//                      char string[] = "some string";                   //
//                      send_to_server(string, strlen(string));          //
//                                                                       //
/*************************************************************************/

int send_to_server(void *data, size_t size){
   
    size_t nb_size = htonl(size);
    
    if(write(sock_fd, &nb_size, sizeof(size)) == -1)
        return -1;

    return write(sock_fd, data, size);

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
//      RP_CONNECT       //
/*************************/

int rp_connect(const char *address, unsigned short port){

    /************************************/
    //         PART 2 OF PROJECT        //                                    
    /************************************/
    
    struct sockaddr_in server_addr;
    struct hostent *host;
    
    // 1. Get the IP Address associated with the address given.
    // Fill in hostent struct by calling gethostbyname()

    host = gethostbyname(address);
    if(!host) {
      exit(1);
    }
    
    // 3. Zero out struct with bzero

    bzero((char *) &server_addr, sizeof(server_addr));
    
    // 4. Set up the rest of the struct fields (family, sin_addr, and port)

    //Says to use ipv4 address
    server_addr.sin_family = (short) AF_INET;

    server_addr.sin_port = port;

    //bcopy copies n bytes from src to dest.
    //[source] [destination] [size]
    bcopy(host->h_addr, (char *) &server_addr.sin_addr, host->h_length);
    
    // 5. Create socket **!! USE GLOBAL sock_fd VARIABLE TO STORE FILE DESCRIPTOR !!** 
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    //bind(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    
    // 6. Connect to server by calling connect()
    int err = connect(sock_fd, (struct sockaddr*) &server_addr, host->h_length);

    // 7. Return 0 on success and return -1 on error
    return err;
}


/*************************/
//        RP_OPEN        //
/*************************/

int rp_open(const char *pathname, int flags, ...) {

    va_list arg_list;

    va_start(arg_list, flags);

    mode_t mode;

    // Check if there will be a mode passed
    // This can be done by chekcing the flags to see if O_CREAT is speacified
    
    if((O_CREAT & flags) > 0) {
      mode = va_arg(arg_list, mode_t);
      
    }

    else {
      mode = 0777;
    }
    
    va_end(arg_list);

    /************************************/
    //         PART 1 OF PROJECT        //                                    
    /************************************/
    
    // Call the syscall with argumnents given, return result to user, and set errno accordingly
    //return open(pathname, flags, mode);

    /************************************/
    //         PART 2 OF PROJECT        //                                    
    /************************************/
    
    // Send type of call to server
    char *buf = "open";
    send_to_server(buf, sizeof(buf));

    char *path = pathname;
    // Send each argument to the server (pathname, flags, mode)
    send_to_server(path, sizeof(path));
    send_to_server(&flags, sizeof(flags));
    
    // Read return of open coming back from server 
    int *file = read_from_server();
    int realFile = *file;
    free(file);
    
    // Read errno sent from server
    int *err = read_from_server();
    int realerr = *err;
    free(err);

    errno = realerr;
    
    return realFile;
    
    // Return the return of open
}


/*************************/
//        RP_CLOSE       //
/*************************/

int rp_close(int fd){
  
  // Send type of call to server
  char *buf = "close";
  send_to_server(buf, sizeof(buf));
  
  // Send each argument to the server (pathname, flags, mode)
  send_to_server(&fd, sizeof(fd));
  
  // Read return of open coming back from server 
  int *result = read_from_server();
  int trueResult = *result;
  free(result);
  
  // Read errno sent from server
  int *err = read_from_server();
  int realerr = *err;
  free(err);
  
  errno = realerr;
  
  return trueResult;

}


/*************************/
//        RP_READ        //
/*************************/

ssize_t rp_read(int fd, void *buf, size_t count) {
  // Send type of call to server
  char *buffer = "read";
  send_to_server(buffer, sizeof(buffer));
  
  // Send each argument to the server
  send_to_server(&fd, sizeof(fd));
  send_to_server(buf, sizeof(buf));
  send_to_server(count, sizeof(count));
  
  // Read return of open coming back from server 
  int *result = read_from_server();
  int trueResult = *result;
  free(result);
  
  // Read errno sent from server
  int *err = read_from_server();
  errno = *err;
  free(err);
  
  // Return result
  return trueResult;
}


/*************************/
//       RP_WRITE        //
/*************************/

ssize_t rp_write(int fd, const void *buf, size_t count) {
  // Send type of call to server
  char *buffer = "write";
  send_to_server(buffer, sizeof(buffer));
  
  // Send each argument to the server
  send_to_server(&fd, sizeof(fd));
  send_to_server(buf, sizeof(buf));
  send_to_server(&count, sizeof(count));
  
  // Read return of open coming back from server 
  int *result = read_from_server();
  int trueResult = *result;
  free(result);
  
  // Read errno sent from server
  int *err = read_from_server();
  errno = *err;
  free(err);
  
  // Return result
  return trueResult;
}


/*************************/
//       RP_LSEEK        //
/*************************/

off_t rp_lseek(int fd, off_t offset, int whence) {
  // Send type of call to server
  char *buffer = "lseek";
  send_to_server(buffer, sizeof(buffer));
  
  // Send each argument to the server
  send_to_server(&fd, sizeof(fd));
  send_to_server(&offset, sizeof(offset));
  send_to_server(&whence, sizeof(whence));
  
  // Read return of open coming back from server 
  int *result = read_from_server();
  int trueResult = *result;
  free(result);
  
  // Read errno sent from server
  int *err = read_from_server();
  errno = *err;
  free(err);
  
  // Return result
  return trueResult;
  
}


/*************************/
//      RP_CHECKSUM      //
/*************************/

short rp_checksum(int fd) {
    unsigned char checksum = 0;
    
    // Send type of call to server
    char *buf = "checksum";
    send_to_server(buf, sizeof(buf));
    
    // Send each argument to the server
    send_to_server(&fd, sizeof(fd));
    
    // Read return of open coming back from server 
    unsigned char *result = read_from_server();
    unsigned char trueResult = *result;
    free(result);
    
    // Read errno sent from server
    int *err = read_from_server();
    errno = *err;
    free(err);
    
    // Return result
    return trueResult;  
}
