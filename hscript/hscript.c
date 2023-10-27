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
  char buf[1000];
  int fd0;
  int fd1;
  int fd2;
  int status = 0;
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
    }

    if(errno == EACCES) {
      strcpy(buf, "MKDIR FAILED: SEARCH OR WRITE PERMISSION DENIED\n");
      write(2, buf, strlen(buf));
    }
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

  
  cpid = fork();
  if(cpid == 0) {
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
  while((wpid = wait(&status)) > 0);
  
  return 0;
}
