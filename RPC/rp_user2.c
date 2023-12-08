#include "rp_client.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  
  if(rp_connect(argv[1], atoi(argv[2]))) {
    write(2, "bad connect\n", sizeof("bad connect\n"));
    return -1;
  }
  

  if(argc < 1) {
    return -1;
  }
  
  int remoteFd = rp_open(argv[3], O_RDONLY);  
  char buf[1024];
  int err;

  int localFd = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0777);

  rp_lseek(remoteFd, 10, SEEK_SET);
  
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
