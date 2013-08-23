#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


/* Pointer to simulator memory */
uint8_t *mem;
/* Create variable to keep track of heap status here 
   You may change this to use your own implementation. */
uint8_t *heap_status;
uint32_t FlagAddr;  // try the extra credit mark the current
/* Called by program loader to initialize memory. */
uint8_t* init_mem() {
  assert (mem == NULL);
  mem = (uint8_t*) calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
  if (mem==NULL) fprintf(stderr, "%s: Allocation Failed\n", __FUNCTION__);

  //you may change the following line if using your own implementation to maintain the heap
  heap_status = (uint8_t*) init_heap_status();
  FlagAddr=HEAP_BOTTOM;
  return (uint8_t*) mem;
}

/* Called in init_mem() to initialize your implementation of
   how you keep track of heap allocation status.*/
uint8_t* init_heap_status(){

  /* TODO YOUR CODE HERE */
   assert (heap_status == NULL);
   int HEAP_SIZE=HEAP_TOP-HEAP_BOTTOM;
  heap_status = (uint8_t*) calloc(HEAP_SIZE, sizeof(uint8_t)); // allocate zeroed memory
  if (heap_status==NULL)
    fprintf(stderr, "%s: Allocation Failed\n", __FUNCTION__);
   
  return heap_status;
}

/* Returns 1 if memory access is ok, otherwise returns 0. 
The parameter write_permission is the macro READ if this is
a read request, and WRITE if it is a write request. See memory.h. */

int access_ok(uint32_t mipsaddr, mem_unit_t size, uint8_t write_permission) {

  /* TODO YOUR CODE HERE */
   if (mipsaddr>=0&& mipsaddr<CODE_BOTTOM)
     return 0; // test reserved memory
   else if ((mipsaddr>=CODE_BOTTOM)&&\
       (mipsaddr<CODE_TOP)&&\
       write_permission==WRITE)
     return 0; // test write to read only area
   else if ((mipsaddr>=HEAP_BOTTOM)&&(mipsaddr<HEAP_TOP)){
      uint8_t* temp=(heap_status+mipsaddr-HEAP_BOTTOM);
      if ((temp[0])==0) return 0; // unallocated
      else if (temp[0]==1){//head
        if (size==1)
          return 1;
        else if (size==2){
           if (temp[1]==2)
             return 1;
           else return 0;
        }
        else if (size==4){
           if (temp[1]==2&&temp[2]==2&&temp[3]==2)
             return 1;
           else return 0;
        } 
        else return 0;
      }
      else if (temp[0]==2){ // middle of array
          if (size==SIZE_BYTE)
          return 1;
        else if (size==SIZE_HALF_WORD){
          if(temp[1]==2)
            return 1; 
          else return 0;
        } else if (size==SIZE_WORD){
          if (temp[1]==2&&temp[2]==2&&temp[3]==2)
                return 1;
          else return 0;

        }else
         return 0;
      }else return 0;
       
   }else if (size==SIZE_WORD &&( mipsaddr%4)!=0)// alignment
     return 0;
   else if (mipsaddr<MEM_SIZE&&mipsaddr>=HEAP_TOP)
     return 1;
   else if (mipsaddr>=MEM_SIZE)
     return 0;
  
  return 1;
}

/* Writes size bytes of value into mips memory at mipsaddr */
void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) {
  if (!access_ok(mipsaddr, size, WRITE)) {
    fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  } 
  
  /* TODO YOUR CODE HERE */
 uint8_t* temp1=mipsaddr+mem;
 if (size==SIZE_BYTE)
   *(temp1)=(uint8_t)value;
 else if (size==SIZE_HALF_WORD){
   uint8_t tempUpper=(value&0x0000ff00)>>8;
   temp1[1]=tempUpper;
   uint8_t tempLower=value&0x000000ff;
   temp1[0]=tempLower;
 }
 else if (size==SIZE_WORD){
   uint8_t tempFirst=(value&0xff000000)>>24;
   temp1[3]=tempFirst;
   uint8_t tempSecond=(value&0x00ff0000)>>16;
   temp1[2]=tempSecond;
   uint8_t tempThird=(value&0x0000ff00)>>8;
   temp1[1]=tempThird;
   uint8_t tempForth=value&0x000000ff;
   temp1[0]=tempForth;
 }
}

/* Returns zero-extended value from mips memory */
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) {
 if (!access_ok(mipsaddr, size, READ)) {
    fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
   
       uint8_t* temp=mem+mipsaddr;
  /* TODO YOUR CODE HERE */
       if (size==SIZE_BYTE)
         return (uint32_t) temp[0];
       else if (size==SIZE_HALF_WORD){
         uint32_t tempReturn=(uint32_t)temp[1];
         tempReturn=(tempReturn<<8)+(uint32_t)temp[0];
         return tempReturn;
       }
       else if (size==SIZE_WORD){
         uint32_t tempReturn=(uint32_t)temp[3];
         tempReturn=(tempReturn<<8)+(uint32_t)temp[2];
         tempReturn=(tempReturn<<8)+(uint32_t)temp[1];
         tempReturn=(tempReturn<<8)+(uint32_t)temp[0];
         return tempReturn;  
       
       }
  // incomplete stub to let mipscode/simple execute
  // (only handles size == SIZE_WORD correctly)
  // feel free to delete and implement your own way
  
  return 0;
}

/*Uses the first fit algorithm to allocate a block of size bytes 
in the heap. Returns the address of the start of the block 
if allocation is successful, returns 0 on failure.*/

uint32_t first_fit_malloc(uint32_t size){

  uint8_t *hs_pointer = heap_status;
  int i=0;
  int HEAP_SIZE=HEAP_TOP-HEAP_BOTTOM;

  while((uint32_t)(hs_pointer-heap_status)<HEAP_SIZE){
    if ((int)hs_pointer[0]==1||(int)hs_pointer[0]==2){
     hs_pointer+=4;
    }
  else {
     for (i=1;hs_pointer[i]==0;i++)
        if (i==(size-1)){
          int j;
          hs_pointer[0]=1;
          for (j=1;j<size;j++)
          hs_pointer[j]=2;
            return (hs_pointer-heap_status)+HEAP_BOTTOM;
        }else if (size==1){
          hs_pointer[0]=1;
          return hs_pointer-heap_status+ HEAP_BOTTOM;
        }
     if (i<size)
     hs_pointer+=4; 
  } 
  }
return 0;
}


/*Free the allocated block of memory in the heap beginning at
mipsaddr. In order to successfully free a block, mipsaddr must 
be the beginning of a block that was allocated on the heap. 
If block_free() is called on an unallocated memory address or an address
that is not the beginning of a block, bad_free() should be called.
*/

void block_free(uint32_t mipsaddr){
  /* TODO YOUR CODE HERE */
  
  uint8_t* temp=heap_status+mipsaddr-HEAP_BOTTOM;
  uint8_t* temp_Mem=mem + mipsaddr;
  //********************************
  //remember to get back HEAP_BOTTOM
  //
  int i=1;
  if (temp[0]!=1) 
   bad_free(mipsaddr);
  else {
    FlagAddr=mipsaddr;
    temp[0]=0;
    temp_Mem[0]=0;
    while(temp[i]!=1&&temp[i]!=0) {
      temp[i]=0;
      temp_Mem[i]=0;
      i++;
    }
  }
  
}



/*Implementation of this function is optional but may be useful for debugging.
  This function will not be graded and is for your benefit only. To call this 
  function in MIPS, use syscall with $v0=64.*/

void print_heap_status(){
  //YOUR CODE HERE
  //OPTIONAL FOR DEBUGGING
}

/*Called when trying to free an invalid address. Do NOT
  change this function.*/
void bad_free(uint32_t mipsaddr){
  fprintf(stderr, "%s: bad free=%08x\n", __FUNCTION__, mipsaddr);
  exit(-1);
}


/**Extra Credit**/
/*Wait until completing the rest of the project to work on this part; it
  requires significantly more work than first_fit_malloc().
  You may need to declare new variables and/or modify functions you 
  have already completed.*/

 

uint32_t next_fit_malloc(uint32_t size){
  /*TODO YOUR CODE HERE*/ 
   static int MaxSpan=1;// if full, go back to begining
  if (!access_ok(FlagAddr, size, READ)) {
      FlagAddr=HEAP_BOTTOM;
      if (MaxSpan==0)  {
        fprintf(stderr,"Sorry, There is a memory overflow\n");
        exit(-1);
      
      }

      MaxSpan--;
   
  }
        
  uint8_t *hs_pointer = heap_status+FlagAddr-HEAP_BOTTOM;
  int i=0;
  int HEAP_SIZE=HEAP_TOP-HEAP_BOTTOM;

  while((uint32_t)(hs_pointer-heap_status)<HEAP_SIZE){
    if ((int)hs_pointer[0]==1||(int)hs_pointer[0]==2){
     hs_pointer+=4;
    }
  else {
     for (i=1;hs_pointer[i]==0;i++)
        if (i==(size-1)){
          int j;
          hs_pointer[0]=1;
          for (j=1;j<size;j++)
          hs_pointer[j]=2;
            MaxSpan=1;
            if (size==4)
          FlagAddr=(hs_pointer-heap_status)+HEAP_BOTTOM+size;
            else    
              FlagAddr=(hs_pointer-heap_status)+HEAP_BOTTOM+\
                       (4-(size%4))+size;
        
            return (hs_pointer-heap_status)+HEAP_BOTTOM;
        }else if (size==1){
          hs_pointer[0]=1;
          MaxSpan=1;
            if (size==4)
          FlagAddr=(hs_pointer-heap_status)+HEAP_BOTTOM+size;
            else    
              FlagAddr=(hs_pointer-heap_status)+HEAP_BOTTOM+\
                       (4-(size%4))+size;
        
            return (hs_pointer-heap_status)+HEAP_BOTTOM;
        }
     if (i<size)
     hs_pointer+=4; 
  } 
  }
  return(uint32_t) 0;
}                    


