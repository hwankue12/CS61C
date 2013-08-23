/*********************************************************************
teststack.c

This file implements a main function that runs some simple tests on
the stack data structure defined in stack.h and stack.c.  This is not
meant to be a useful test of the stack.  Instead, it is just going to
be used to demonstrate linker and object file features.

**********************************************************************/

#include<stdio.h>
#include<stdlib.h>

/* Include the definitions for the stack data structure */
#include "stack.h"

int main () {
    /* Initiailize some values to test the stack */
    int value1 = 7;
    int value2 = 2;

    /* Initialize the stack and get a pointer to it */
    Stack s = EmptyStack();

    printf("Value2 before: %d\n", value2);

    /* play with the stack */    
    printf("Pushing Value2");
    Push(s, value2);
    
    if (IsEmpty (s)) {
	Push (s, value1);
    } else {
      printf("Popping Value2");
      value2 = Pop(s);
    }

    printf("Value2 after: %d\n", value2);

    return EXIT_SUCCESS;
}
