#include "hmalloc.h"
/*You may include any other relevant headers here.*/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  // some calls to hmalloc
  char *a;
  char *b;
  int *c;
  int *d;
  a = hmalloc(10);
  b = hmalloc(sizeof(char));
  c = hmalloc(sizeof(int));
  d = hmalloc(55);
  *a = 5;
  *b = 6;
  *c = 992;
  *d = 555555;
  printf("%d\n", *a);
  
  printf("area [0-3] = %d, region in hex = %p\n", *((char*) (a-8)), ((char*) (a-8)));
  printf("area [4-7] = %d, region in hex = %p\n", *((char*) (a-4)), ((char*) (a-4)));
  
  printf("%d\n", *b);
  
  printf("area [0-4] = %d, region in hex = %p\n", *((char*) (b-8)), ((char*) (b-8)));
  printf("area [4-7] = %d, region in hex = %p\n", *((char*) (b-4)), ((char*) (b-4)));
  
  printf("%d\n", *c);
  
  printf("area [0-4] = %d, region in hex = %p\n", *((char*) (c) - 8), ((char*) (c-2)));
  printf("area [4-7] = %d, region in hex = %p\n", *((char*) (c) - 4), ((char*) (c-1)));
  
  printf("%d\n", *d);
  
  printf("area [0-4] = %d, region in hex = %p\n", *((char*) (d) - 8), ((char*) (d-2)));
  printf("area [4-7] = %d, region in hex = %p\n", *((char*) (d) - 4), ((char*) (d-1)));
  // some calls to hfree
  hfree(a);
  hfree(b);
  hfree(c);
  hfree(d);
  printf("Length between pointers: %d\n", *((char *) (a) - 4));
  printf("Length between pointers: %d\n", *((char *) (b) - 4));
  printf("Length between pointers: %d\n", *((char *) (c) - 4));
  printf("Length between pointers: %d\n", *((char *) (d) - 4));
  traverse();
  a = hmalloc(10);
  *a = 240;
  traverse();
  b = hmalloc(sizeof(char));
  *b = 23;
  traverse();
  c = hmalloc(sizeof(int));
  *c = 69;
  traverse();
  d = hmalloc(55);
  *d = 5555;
  traverse();
  
  return 1;
}
