#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
  pid_t cpid, wpid;
  char buf[1000];
  int status = 0;
  if(argc < 3) {
    strcpy(buf, "./hscript [program name] [arguments] [directory]\n");
    write(2, buf, strlen(buf));
    return 0;
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
    write(2, "UH OH\n", strlen("UH OH\n"));
    exit(0);
  }
  while((wpid = wait(&status)) > 0);
  
  return 0;
}
