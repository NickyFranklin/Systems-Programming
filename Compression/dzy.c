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
	
	//It is recommeded that you implement the bit abstractions in bitsy.c and
	//utilize them to implement your decoder.
	//If so, do NOT call read/write here. Instead rely exclusively on 
	//read_bit, read_byte, write_bit, write_byte, and flush_write_buffer.
	
	return 0; //exit status. success=0, error=-1
}
