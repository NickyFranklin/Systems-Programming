#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
  char buf[1024];
  for(int j = 0; j < 5; j++) {
    for(int i = 0; i < 1000; i++) {
      write(1, "a", 1);
    }
    write(1, "\n", 1);
  }

  read(0, buf, 1024); 
  write(2, buf, 1024);
  
  return 0;
}
