#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
  char buf[1024];
  for(int j = 0; j < 5; j++) {
    for(int i = 0; i < 500; i++) {
      write(1, "a", 1);
    }
    write(1, "\n", 1);
  }
  int error;
  error = read(0, buf, 1024);
  if(error < 0) {
    write(2, "ERROR", sizeof("ERROR"));
  }
  //write(2, buf, strlen(buf));
  
  return 0;
}
