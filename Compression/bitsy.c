#include "bitsy.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Add any global variables or structures you need.*/
typedef struct {
  unsigned short byte;
  unsigned short bit[8];
  unsigned char location;
  unsigned char byteSet;
} Byte;

Byte readByte = {.byteSet = 0};
Byte writeByte = {.byteSet = 0, .location = 0, .byte = 0};
/* read_byte
 * Abstraction to read a byte.
 * Relys on readBit.
 */
unsigned short read_byte(){ 
  /* This function should not call read() directly.
   * If we are buffering data in read_bit, we dont want to skip over
   * that data by calling read again. Instead, call read_bit and 
   * construct a byte one bit at a type. This also allows us to read
   * 8 bits that are not necessarily byte alligned. */
  //Do NOT call read here. Instead rely on read_bit.
  //I suggest returning a unique value greater than the max byte value
  //to communicate end of file. We need this since 0 is a valid byte value
  //so we need another way to communicate eof. These functions are typed
  //as short to allow us to return a value greater than the max byte value.

  short returnByte = 0;
  short readBit = 2;
  for(int i = 0; i < 8; i++) {
    readBit = read_bit();
    //If the bit we read in is greater than 1, its eof
    //Behavior can change to be either only the proper data followed by 1s, 0s, or return a greater
    //value
    if(readBit > 1) {
      return 512;
      //returnByte = returnByte << (7 - i);
    }
    
    //else treat as normal
    returnByte |= readBit;
   

    //Shift over for the next bit
    returnByte = returnByte << 1;
  }

  //shift back over to undo the final shift
  returnByte = returnByte >> 1;
  return returnByte;
}

/* read_bit
 * Abstraction to read a bit.
 * */
unsigned short read_bit(){
  /* This function is responsible for reading the next bit on the
   * input stream from stdin (fd = 0). To accomplish this, keep a 
   * byte sized buffer. Each time read bit is called, use bitwise
   * operations to extract the next bit and return it to the caller.
   * Once the entire buffered byte has been read the next byte from 
   * the file. Once eof is reached, return a unique value > 255
   *
   */	
  //You will need to call read here

  //If the byte is old/has not been set, then read in one byte
  short eof = 0;  
  if(readByte.byteSet == 0) {
    //If end of file reached, return a number higher than a char can
    readByte.byteSet = 1;
    eof = read(0, &(readByte.byte), 1);
    
    if(!eof) {
      readByte.byteSet = 0;
      return 512;
    }

    //Set the location to be at the beginning bit
    readByte.location = 8;
    //Set each bit in the array
    int j = 1;
    for(int i = 0; i < 8; i++) {
      readByte.bit[i] = ((readByte.byte) & j) >> i;
      j = j << 1;
    }
  }
  
  //if the byte has already been set, set location forward
  if(readByte.byteSet == 1) {
    readByte.location--;
  }

  //If at the end of the array, put set array to 0
  if(readByte.location == 0) {
    readByte.byteSet = 0;
  }
  
  //Return the current bit
  return readByte.bit[readByte.location];
}

/* write_byte
 * Abstraction to write a byte.
 */
void write_byte(unsigned char byt){
  /* Use writeBit to write each bit of byt one at a time. Using writeBit
   * abstracts away byte boundaries in the output.*/
  for(int i = 0; i < 8; i++) {
    write_bit((byt >> (7 - i)) & 1);
  }
  
  //Do NOT call write, instead utilize writeBit()
}

/* write_bit
 * Abstraction to write a single bit.
 */
void write_bit(unsigned char bit){
  /* Keep a byte sized buffer. Each time this function is called, insert the 
   * new bit into the buffer. Once 8 bits are buffered, write the full byte
   * to stdout (fd=1).
   */
  //Write bits into their respective locations and increase the location int for where they are
  writeByte.bit[writeByte.location] = bit;
  writeByte.location++;
  
  //You will need to call write here eventually.
  //If the spaces for the bits have been filled, construct the byte and write it to stdout
  if(writeByte.location == 8) {
    //Set location back to 0 for the next time write bit is called
    writeByte.location = 0;
    //Write each part of the byte bit by bit
    for(int i = 0; i < 8; i++) {
      writeByte.byte |= writeByte.bit[i];
      writeByte.byte = writeByte.byte << 1;
    }
    //Undo the final bit shift and write to stdout
    writeByte.byte = writeByte.byte >> 1;
    write(1, &(writeByte.byte), 1);

    //Set the byte back to 0
    writeByte.byte = 0;
  }
}

/* flush_write_buffer
 * Helper to write out remaining contents of a buffer after padding empty bits
 * with 1s.
 */
void flush_write_buffer(){
  /* This will be utilized when finishing your encoding. It may be that some bits
   * are still buffered and have not been written to stdout. Call this function 
   * which should do the following: Determine if any buffered bits have yet to be 
   * written. Pad remaining bits in the byte with 1s. Write byte to stdout
   */
  if(writeByte.location > 0) {
    for(int i = writeByte.location; i < 8; i++) {
      write_bit(1);
    }
  }
}
