#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {
  pid_t cpid, wpid;
  int error;
  int nread = 0;
  char buf[1000];
  char bufRead[1024];
  int fd0, fd1, fd2, fd0PtC[2], fd1CtP[2], fd2CtP[2];
  int status = 0;
  if(argc < 3) {
    strcpy(buf, "./hscript [program name] [arguments] [directory]\n");
    write(2, buf, strlen(buf));
    return 1;
  }

  if(pipe(fd0PtC) < 0 || pipe(fd1CtP) < 0 || pipe(fd2CtP) < 0) {
    strcpy(buf, "PIPING FAILED\n");
    write(2, buf, strlen(buf));
    return 1;
  }

  if(fcntl(fd0PtC[0], F_SETFL, O_NONBLOCK) < 0 ||
     fcntl(fd1CtP[0], F_SETFL, O_NONBLOCK) < 0 ||
     fcntl(fd2CtP[0], F_SETFL, O_NONBLOCK) < 0) {
    strcpy(buf, "PIPING SETTINGS FAILED\n");
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
  
  
  //strcpy(buf, argv[argc-1]);
  sprintf(buf, "%s/0", argv[argc-1]);
  //write(1, buf, strlen(buf));
  //write(1, "\n", strlen("\n"));
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

  fd_set write_set;
  FD_ZERO(&write_set);
  FD_SET(fd0PtC[1], &write_set);

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd1CtP[0], &read_set);
  FD_SET(fd2CtP[0], &read_set);
  
  cpid = fork();
  if(cpid == 0) {
    //dup the file descriptors
    if(dup2(fd0PtC[0], 0) < 0 ||
       dup2(fd1CtP[1], 1) < 0 ||
       dup2(fd2CtP[1], 2) < 0) {
      strcpy(buf, "DUP FAILED\n");
      write(2, buf, strlen(buf));
      return 1;
    }
    if(close(fd0PtC[1]) < 0 ||
       close(fd1CtP[0]) < 0 ||
       close(fd2CtP[0]) < 0) {
      strcpy(buf, "CLOSE FAILED\n");
      write(2, buf, strlen(buf));
      return 1;
    }
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
  while((wpid = wait(&status)) > 0) //|| select(fd2CtP[1] + 1, &read_set, &write_set, NULL, NULL) > 0)
    {
    //Stdout
    FD_ZERO(&write_set);
    FD_SET(fd0PtC[1], &write_set);
    nread = read(fd1CtP[0], bufRead, 1024);
    if(nread < 0) {
      if(errno == EAGAIN) {
	
      }

      else {
	strcpy(buf, "READ FAILED\n");
	write(2, buf, strlen(buf));
      }
    }
    
    else {
      write(fd1, bufRead, strlen(bufRead));
      write(1, bufRead, strlen(bufRead));
    }

    //stderr
    nread = read(fd2CtP[0], bufRead, 1024);
    if(nread < 0) {
      if(errno == EAGAIN) {
	
      }

      else {
	strcpy(buf, "READ FAILED\n");
	write(2, buf, strlen(buf));
      }
    }
    
    else {
      write(fd2, bufRead, strlen(bufRead));
      write(2, bufRead, strlen(bufRead));
    }

    //stdin
    /*
    if(select(fd0PtC[1] + 1, NULL, &write_set, NULL, NULL) > 0) {
      if(FD_ISSET(fd0PtC[1], &write_set)) {
	nread = read(0, bufRead, 1022);
	bufRead[nread] = '\0'; 
	write(fd0, bufRead, strlen(bufRead));
	//write(0, bufRead, strlen(bufRead));
	write(fd0PtC[0], bufRead, strlen(bufRead));
      }
    }
    FD_SET(fd1CtP[0], &read_set);
    FD_SET(fd2CtP[0], &read_set);
  
    */
    }  
  return 0;
}
