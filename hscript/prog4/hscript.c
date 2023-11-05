#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <poll.h>

int main(int argc, char** argv) {
  int error;
  char buf[1000];
  char readBuf[1026];
  int num = 0;
  int fd0, fd1, fd2, fdStdin[2], fdStdout[2], fdStderr[2];
  pid_t cpid;
  int nread, nread2, nread3 = nread2 = nread = 0;

  signal(SIGCHLD, SIG_IGN);
  
  if(argc < 3) {
    strcpy(buf, "./hscript [program name] [arguments] [directory]\n");
    write(2, buf, strlen(buf));
    return 1;
  }
  
  error = mkdir(argv[argc-1], 0777);
  if(error != 0) {
    if(errno == EEXIST) {
      strcpy(buf, "MKDIR FAILED: DIRECTORY ALREADY EXISTS\n");
      write(2, buf, strlen(buf));
      return 1;
    }

    if(errno == EACCES) {
      strcpy(buf, "MKDIR FAILED: SEARCH OR WRITE PERMISSION DENIED\n");
      write(2, buf, strlen(buf));
      return 1;
    }
    strcpy(buf, "MKDIR FAILED\n");
    write(2, buf, strlen(buf));
    return 1;
  }
  
  sprintf(buf, "%s/0", argv[argc-1]);
  fd0 = open(buf, O_CREAT | O_TRUNC | O_RDWR, 0777);
  sprintf(buf, "%s/1", argv[argc-1]);
  fd1 = open(buf, O_CREAT | O_TRUNC | O_RDWR, 0777);
  sprintf(buf, "%s/2", argv[argc-1]);
  fd2 = open(buf, O_CREAT | O_TRUNC | O_RDWR, 0777);
  if(error != 0) {
    strcpy(buf, "OPEN FAILED: ONE OF THE FILES COULD NOT BE OPENED\n");
    write(2, buf, strlen(buf));
    return 1;
  }


  pipe(fdStdin);
  pipe(fdStdout);
  pipe(fdStderr);

  //fcntl(fdStdout[0], O_NONBLOCK);
  //fcntl(fdStderr[0], O_NONBLOCK);

  
  cpid = fork();
  if(cpid == 0) {
    //dup the file descriptors
    close(fdStdin[1]);
    close(fdStdout[0]);
    close(fdStderr[0]);
    
    dup2(fdStdin[0], 0);
    dup2(fdStdout[1], 1);
    dup2(fdStderr[1], 2);
    
    //Exec Call schenanigans
    char *args[argc-1];
    for(int i = 0; i < argc - 2; i++) {
      args[i] = (char*) malloc(100);
      args[i] = argv[i+1];
    }
    args[argc-2] = NULL;
    execvp(args[0], args);
    strcpy(buf, "EXEC FAILED\n");
    write(2, buf, strlen(buf));
    exit(1);
  }

  else {
    close(fdStdin[0]);
    close(fdStdout[1]);
    close(fdStderr[1]);
  }

  if(kill(cpid, 0) < 0) {
      return 0;
  }
  
  ioctl(0, FIONREAD, &nread);
  if(nread > 0) {
    num = read(0, readBuf, 1024);
    write(fd0, readBuf, num);
    write(fdStdin[1], readBuf, num);
  }

  //write(1, "1\n", sizeof("1\n"));
  ioctl(fdStdout[0], FIONREAD, &nread2);
  if(nread2 > 0) {
    num = read(fdStdout[0], readBuf, 1024);
    write(fd1, readBuf, num);
    write(1, readBuf, num);
  }

  //write(1, "1\n", sizeof("1\n"));
  ioctl(fdStderr[0], FIONREAD, &nread3);
  if(nread3 > 0) {
    num = read(fdStderr[0], readBuf, 1024);
    write(fd2, readBuf, num);
    write(2, readBuf, num);
  }
  
  while(1) {
    //write(1, "1\n", sizeof("1\n"));
    if(kill(cpid, 0) < 0) {
      return 0;
    }
    
    ioctl(0, FIONREAD, &nread);
    if(nread > 0) {
      num = read(0, readBuf, 1024);
      write(fd0, readBuf, num);
      write(fdStdin[1], readBuf, num);
    }

    //write(1, "2\n", sizeof("1\n"));
    ioctl(fdStdout[0], FIONREAD, &nread2);
    if(nread2 > 0) {
      num = read(fdStdout[0], readBuf, 1024);
      write(fd1, readBuf, num);
      write(1, readBuf, num);
    }

    //write(1, "3\n", sizeof("1\n"));
    ioctl(fdStderr[0], FIONREAD, &nread3);
    if(nread3 > 0) {
      num = read(fdStderr[0], readBuf, 1024);
      write(fd2, readBuf, num);
      write(2, readBuf, num);
    }
        
  }  
  return 0;
}
