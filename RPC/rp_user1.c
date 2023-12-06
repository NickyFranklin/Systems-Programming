#include "rp_client.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  
  
  int remoteFd = rp_open("rp_client.c", O_RDONLY);

  char buf[1024];
  int err;
  
  int localFd = open("user1File.txt", O_TRUNC | O_CREAT | O_RDWR, 0777);

  
  err = rp_read(remoteFd, buf, 1024);
  while(err > 0) {
    err = write(localFd, buf, err);
    
    if(err < 1024) {
      break;
    }
    
    err = rp_read(remoteFd, buf, 1024);
  }

  rp_close(remoteFd);
  close(localFd);
  
  return 0;
}
