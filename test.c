#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int func(int x) {
  func(x);
  return -1;
}

int main() {
  func(1);
  
  return 0;
}

