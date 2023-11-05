#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
  char buf[1024];
  int num;
  for(int i = 0; i < 3; i++) {
    num = read(0, buf, 1024);
    buf[num] = '\0';
    write(1, buf, strlen(buf));
    write(2, buf, strlen(buf));
  }
  
  write(1, "Balls\n", sizeof("Balls\n"));
  
  return 0;
}
