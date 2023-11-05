#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int fdStdIn[2];
int fdStdOut[2];
int fdStdErr[2];
pid_t cpid;

void handler() {
  char buf[1024];
  int num;

  if(!kill(cpid, 0)) {
    num = read(0, buf, 1024);
    buf[num] = '\0';
    write(fdStdIn[1], buf, strlen(buf));
  }
}


int main() {
  struct sigaction handlerr;
  handlerr.sa_handler = handler;
  
  sigaction(SIGCHLD, &handlerr, NULL);
  

  return 0;
}
