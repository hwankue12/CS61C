#include<stdio.h>
#include<stdlib.h>
#include"memory.h"
#include"processor.h"
extern uint8_t* mem;
extern uint8_t* heap_status;
uint32_t SignExt_16_32(uint16_t input){
    uint16_t match=input>>15;
    uint32_t result;
    if (match!=0)
      result=(0xffff0000|(uint32_t)input);
    else
      result=(uint32_t) input;
    return result;
}
#define SIZE 40
void printfArray(uint8_t* Array,uint8_t* Array_Mem){
   int i=0;
   fprintf(stderr,"This is Stored now %d %d %d %d %d ",Array_Mem[0],Array_Mem[1]\
       ,Array_Mem[2],Array_Mem[3],Array_Mem[4]);
   for (i=5;i<SIZE;i++)
     fprintf(stderr," %d ",Array_Mem[i]);
   fprintf(stderr,"\n");
    fprintf(stderr,"This is status %d %d %d %d %d\n",Array[0],Array[1]\
       ,Array[2],Array[3],Array[4]);
    for (i=5;i<SIZE;i++)
     fprintf(stderr," %d ",Array[i]);
   fprintf(stderr,"\n");
  
}

int main(){

  uint16_t temp1=0xffff;
  uint32_t temp2=(int16_t)temp1;
  fprintf(stderr,"temp1:%d, temp2:%d\n",temp1,temp2);
  temp2=SignExt_16_32(temp1);
  fprintf(stderr,"temp1:%d, temp2:%d\n",temp1,temp2);

  uint32_t  mipsaddr;
  uint8_t Array[SIZE];
  int i;
  for (i=0;i<SIZE;i++)
    Array[i]=0;
  Array[0]=1;
  Array[4]=1;
  heap_status=Array;
 uint8_t Array_Mem[SIZE];
  
  for (i=0;i<SIZE;i++)
    Array_Mem[i]=i*2;
  mem=Array_Mem;              
   printfArray(Array,Array_Mem);
   uint32_t temp;
   fprintf(stderr,"The allocated addres is %d \n ",\
      (temp= first_fit_malloc(5)));
    fprintf(stderr,"The allocated addres is %d \n ",\
      (temp= first_fit_malloc(7)));
    block_free(temp);
     fprintf(stderr,"The allocated addres is %d \n ",\
      (temp= first_fit_malloc(9)));
  
   /*
    * after func
    * */
//    store_mem(3,1,255);
  //  fprintf(stderr,"Loaded : %d\n",load_mem(3,1));
    // second alloc
   printfArray(Array,Array_Mem);
 return 0;
}
