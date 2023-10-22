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
  //initial condition
  unsigned short previousByte[8];
  char previousByteLength = 0;
  unsigned short currentByte = 0;
  char isFrequent = 0;
  char frequentByteLocation = 0;

  //While not eof
  while(currentByte < 256) {
    currentByte = read_byte();
    //determine if byte has been seen before
    for(int i = 0; i < previousByteLength; i++) {
      if(currentByte == previousByte[i]) {
	isFrequent = 1;
	frequentByteLocation = i;
      }
    }

    //if byte has been seen, execute algorithm
    if(isFrequent) {
      write_bit(0);
      //write location of previous byte
      write_bit((frequentByteLocation >> 2) & 1);
      write_bit((frequentByteLocation >> 1) & 1);
      write_bit(frequentByteLocation & 1);

      //move things forward
      for(int i = previousByteLength; i > 0; i--) {
	previousByte[i] = previousByte[i-1];
      }

      previousByte[0] = currentByte;

      if(previousByteLength < 7) {
	previousByteLength++;
      }
      
      isFrequent = 0;
    }

    else if(currentByte < 256) {
      write_bit(1);
      write_byte(currentByte);
      
      for(int i = previousByteLength; i > 0; i--) {
	previousByte[i] = previousByte[i-1];
      }
      
      previousByte[0] = currentByte;
      if(previousByteLength < 7) {
	previousByteLength++;
      }
      
    }
    
    
  }
  flush_write_buffer();
  return 0; //exit status. success=0, error=-1
}
