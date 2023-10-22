#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int blockSize = 0;
  char* buffer = (char*) calloc(1000, 1);
  if(argc < 3) {
    sprintf(buffer, "./copy [infile] [outfile] [blocksize]\n");
    write(2, buffer, strlen(buffer));
    return 0;
  }
  
  if(argc < 4) {
    blockSize = 1024;
  }

  else {
    blockSize = atoi(argv[3]);
    if(blockSize % 4 != 0) {
      if(blockSize % 4 == 1) {
	blockSize += 3;
      }
      
      else if(blockSize % 4 == 2) {
	blockSize += 2;
      }
      
      else if(blockSize % 4 == 3) {
	blockSize++;
      }
      
      sprintf(buffer, "Blocksize specified is not a multiple of 4, it will be rounded up to %d\n",
	      blockSize);
      write(2, buffer, strlen(buffer));
    }
  }
  
  char *infile = (char *) calloc(500, 1);
  char *outfile = (char *) calloc(500, 1);
  
  int infileLength = strlen(argv[1]);
  int outfileLength = strlen(argv[2]);

  for(int i = 0; i < infileLength; i++) {
    infile[i] = argv[1][i];
  }
  
  for(int i = 0; i < outfileLength; i++) {
    outfile[i] = argv[2][i];
  }
  
  int infileFD = open(infile, O_RDWR);
  if(infileFD < 0) {
    write(2, "Failed to open infile\n", strlen("Failed to open infile\n"));
    return 0;
  }
  
  int outfileFD = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 0600);
  if(outfileFD < 0) {
    write(2, "Failed to open outfile\n", strlen("Failed to open outfile\n"));
    return 0;
  }

  void *buff = calloc(blockSize, sizeof(unsigned int));
  
  int l;
  int size = 0;
  unsigned int sum = 0;
  l = 1;
  while(l > 0) {
    for(int i = 0; i < (blockSize / 4); i++) {
      if(i == 0) {
	for(int j = 0; j < (blockSize/4); j++) {
	  *((unsigned int*) (buff + j*4)) = 0;
	}
      }
      l = read(infileFD, ((unsigned int*) (buff + i*4)), 4);
      size += l;
      if(l == 1) {
	//printf("last: %08x\n", *((unsigned int*) (buff + i*4)));
	//*((unsigned int*) (buff + i*4)) = *((unsigned int*) (buff + i*4)) << 24;
	//printf("last: %08x\n", *((unsigned int*) (buff + i*4)));
	//sum ^= *((unsigned int*) (buff + i*4));
      }

      else if(l == 2) {
	//*((unsigned int*) (buff + i*4)) = *((unsigned int*) (buff + i*4)) << 16;
      }

      else if(l == 3) {
	//*((unsigned int*) (buff + i*4)) = *((unsigned int*) (buff + i*4)) << 8;
      }
      sum ^= *((unsigned int*) (buff + i*4));
    }
    write(outfileFD, buff, size);
    size = 0;
    sprintf(buffer, "%08x ", sum);
    write(1, buffer, sizeof(buffer));
    write(1, " ", sizeof(" "));
    sum = 0;
  }
  write(1, "\n", sizeof("\n"));

  
  close(infileFD);
  close(outfileFD);
  free(buffer);
  free(infile);
  free(outfile);
  free(buff);
  return 0;
}
