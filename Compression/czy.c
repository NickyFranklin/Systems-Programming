#include "bitsy.h" //This header includes prototypes for the proposed bit abstractions
#include <stdio.h>
#include <stdlib.h>
/*Include any additional headers you require*/
//extern byte;
/*You may use any global variables/structures that you like*/

/* main - czy compression implementation
 * Develop a program called czy which compresses the data stream directed 
 * at its standard input and writes the compressed stream to its standard 
 * output.
 *
 * The compression algorithm reads the input one symbol (i.e., byte) at a 
 * time and compares it with each of the 8 bytes previously seen.  It also 
 * checks to see if the following n characters are the same as the current 
 * symbol.  If the byte has been previously seen, it outputs the position of 
 * the previous byte relative to the current position, Otherwise, the symbol 
 * is output as is by prefixing it with a binary one.
 *
 * To compile czy: make czy
 * To execute: ./czy < somefile.txt > somefile.encoded
 */
int main(int argc, char *argv[]){
  //The implementation of your encoder should go here.
  
  int a[8];
  for(int j = 0; j < 2; j++) {
    for(int i = 0; i < 8; i++) {
      a[i] = read_bit();
      //printf("%d\n", a[i]);
    }
    for(int i = 0; i < 8; i++) {
      printf("%d ", a[i]);
    }
    printf("\n");
  }
  
  
  //char b = read_byte();
  //printf("%d\n", b);
  //It is recommeded that you implement the bit abstractions in bitsy.c and
  //utilize them to implement your encoder. 
  //If so, do NOT call read/write here. Instead rely exclusively on 
  //read_bit, read_byte, write_bit, write_byte, and flush_write_buffer.
  
  return 0; //exit status. success=0, error=-1
}
