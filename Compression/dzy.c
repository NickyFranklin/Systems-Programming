#include "bitsy.h" //This header includes prototypes for the proposed bit abstractions
/*Include any additional headers you require*/

/*You may use any global variables/structures that you like*/

/* main - dzy de-compression implementation
 * This program decompresses a compressed stream directed at its standard input 
 * and writes decompressed data to its standard output.
 *
 * To compile dzy: make dzy
 * To execute: ./dzy < somefile.encoded > somefile_decoded.txt
 */
int main(int argc, char *argv[]){
  //The implementation of your decoder should go here.
  unsigned short currentByte = 0;
  unsigned short previousByte[8];
  unsigned short previousByteLength = 0;
  
  currentByte = read_bit();
  //If not eof, then do the following
  while(currentByte < 256) {
    //If the bit read was an infrequent character, then do that
    if(currentByte == 1) {
      //Read byte and if it isn't eof, continue
      currentByte = read_byte();
      if(currentByte < 256) {
	write_byte(currentByte);

	//Move byte array values up
	for(int i = previousByteLength; i > 0; i--) {
	  previousByte[i] = previousByte[i-1];
	}
	
	if(previousByteLength < 7) {
	  previousByteLength++;
	}
	
	previousByte[0] = currentByte;
      }
      
    }


    //If the bit read was a frequent character, then do that
    else if(currentByte == 0) {
      //get distance from the byte
      currentByte |= read_bit();
      currentByte = currentByte << 1;
      currentByte |= read_bit();
      currentByte = currentByte << 1;
      currentByte |= read_bit();
      write_byte(previousByte[currentByte]);

      //Set current byte to the one we care about
      currentByte = previousByte[currentByte];
      for(int i = previousByteLength; i > 0; i--) {
	previousByte[i] = previousByte[i-1];
      }
      
      if(previousByteLength < 7) {
	previousByteLength++;
      }
      
      previousByte[0] = currentByte;
    }

    currentByte = read_bit();
  }
  
  return 0;
}
