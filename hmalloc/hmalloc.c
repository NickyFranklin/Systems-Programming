#include "hmalloc.h"
#include <stdlib.h>
/*You may include any other relevant headers here.*/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
/*Add additional data structures and globals here as needed.*/
void *free_list = NULL;

/* traverse
 * Start at the free list head, visit and print the length of each
 * area in the free pool. Each entry should be printed on a new line.
 */
void traverse(){
  /* Printing format:
   * "Index: %d, Address: %08x, Length: %d\n"
   *    -Index is the position in the free list for the current entry. 
   *     0 for the head and so on
   *    -Address is the pointer to the beginning of the area.
   *    -Length is the length in bytes of the free area.
   */
  if(free_list != NULL) {
    //this traverses the list until its done
    void* temp = free_list;
    temp = ((char*) (temp - 4));
    int i = 0;
    while(*((char*) (temp)) != 0) {
      printf("Index: %d, Address: %08x, Length: %d\n", i, ((char*) temp - 4),
	     *((char*) temp - 4));
      i++;
      temp = (((char*) temp) - *((char*) (temp)));
    }
    printf("Index: %d, Address: %08x, Length: %d\n", i, ((char*) temp - 4),
	   *((char*) temp - 4));
  }
  else {
    printf("NULL\n");
  }
}

/* hmalloc
 * Allocation implementation.
 *    -will not allocate an initial pool from the system and will not 
 *     maintain a bin structure.
 *    -permitted to extend the program break by as many as user 
 *     requested bytes (plus length information).
 *    -keeps a single free list which will be a linked list of 
 *     previously allocated and freed memory areas.
 *    -traverses the free list to see if there is a previously freed
 *     memory area that is at least as big as bytes_to_allocate. If
 *     there is one, it is removed from the free list and returned 
 *     to the user.
 */
void *hmalloc(int bytes_to_allocate){
  //If the free list is empty, don't bother looking and just allocate memory
  //If the bytes to allocate are less than zero, return a pointer with nothing in it
  if(bytes_to_allocate < 0) {
    printf("Please enter in a positive integer\n");
    void *pb;
    return pb;
  }
  if(free_list == NULL) {
    void *pb = sbrk(bytes_to_allocate + 8);
    uint32_t length = bytes_to_allocate;
    uint32_t next = 0;
    *((uint32_t *) pb) = length;
    *((uint32_t *) (pb + 4)) = next;
    pb = ((uint32_t *) (pb + 8));
    pb = (void *) pb;
    return pb;
  }

  //If the free list isn't null, look through the list
  void* temp = free_list;
  temp = ((char*) (temp - 4));
  int extra = 0;
  void* previous = NULL;
  while(*((char*) (temp)) != 0 || extra != 1) {
    if(*((char*) (temp)) == 0) {
      extra = 1;
    }
    int size = *((char*) (temp - 4));
    //If space is found, allocate it to that block
    if(bytes_to_allocate <= size) {
      //delete if middle
      if(previous != NULL) {
	*((char*) previous) = *((char *) temp) + *((char *) previous);
	if(*((char *) temp) == 0) {
	  free_list = previous;
	  *((char *) (free_list)) = 0;
	}
	else {
	  free_list = previous;
	}
	free_list = ((char*) (free_list + 4));
      }

      //delete if first
      if(previous == NULL) {
	free_list = (((char*) temp) - *((char*) (temp)));
	if(*((char*) free_list) == 0) {
	  free_list = NULL;
	}
	else {
	  free_list = ((char*) (free_list + 4));
	}
      }
      
      return ((char *) (temp + 4));
    }
    previous = temp;
    temp = (((char*) temp) - *((char*) (temp)));
  }

  //If no space is found, make a new memory block
  void *pb = sbrk(bytes_to_allocate + 8);
  uint32_t length = bytes_to_allocate;
  uint32_t next = 0;
  *((uint32_t *) pb) = length;
  *((uint32_t *) (pb + 4)) = next;
  pb = ((uint32_t *) (pb + 8));
  pb = (void *) pb;
  return pb;
}

/* hcalloc
 * Performs the same function as hmalloc but also clears the allocated 
 * area by setting all bytes to 0.
 */
void *hcalloc(int bytes_to_allocate){
  void* pb = hmalloc(bytes_to_allocate);
  //Set every byte to 0
  for(int i = 0; i < bytes_to_allocate; i++) {
    *((char*) (pb + i)) = 0;
  }
  return pb; //placeholder to be replaced by proper return value
}

/* hfree
 * Responsible for returning the area (pointed to by ptr) to the free
 * pool.
 *    -simply appends the returned area to the beginning of the single
 *     free list.
 */
void hfree(void *ptr){
  void* freelistTemp = free_list;
  free_list = ptr;
  uint32_t length = ((char*) free_list) - ((char*) freelistTemp);
  if(freelistTemp != NULL) {
    *((uint32_t *) (free_list - 4)) = length;
  }
}

/* For the bonus credit implement hrealloc. You will need to add a prototype
 * to hmalloc.h for your function.*/

/*You may add additional functions as needed.*/
