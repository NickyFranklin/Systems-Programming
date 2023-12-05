#include "rp_client.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int remoteFd = rp_open("", O_RDONLY);

  char buf[1024];
  int localFd = open("user1File.txt", O_TRUNC | O_CREAT | O_RDWR, 0777);

  err = rp_lseek(remoteFd, 0, SEEK_SET);
  
  err = rp_read(remoteFd, buf2, 1024);
  printf("number read: %d\n", err);

  err = write(localFd, buf2, strlen(buf2));
  printf("number wrote: %d\n", err);

  rp_close(remoteFd);
  close(localFd);
  
  return 0;
}
