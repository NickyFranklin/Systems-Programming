#include "hmalloc.h"
/*You may include any other relevant headers here.*/
#include <stdlib.h>
#include <stdio.h>

/*	main()
 *	Use this function to develop tests for hmalloc. You should not 
 *	implement any of the hmalloc functionality here. That should be
 *	done in hmalloc.c
 *	This file will not be graded. When grading I will replace main 
 *	with my own implementation for testing.*/
int main(int argc, char *argv[]){
  // some calls to hmalloc
  char *a;
  a = hmalloc(10);
  *a = 5;
  printf("%d\n", *a);
  // some calls to hfree
  
  return 1;
}
