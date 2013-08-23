/*********************************************************************
stack.c

This file implements a very simple stack data structure that will be
used to demonstrate linker and object file features.

**********************************************************************/

/* Include the functional prototypes and type definitions */
#include "stack.h"

/* Create the stack structure.  Note that this structure is static
   because it is not defined inside a function */
static struct stack {
    int stack [9];
    int nextEmpty;
} ourStack;

/* Stack manipulation functions */
Stack EmptyStack () {
    ourStack.nextEmpty = 0;
    return &ourStack;
}

int IsEmpty (Stack s) {
    return (s->nextEmpty == 0);
}

void Push (Stack s, int n) {
    s->stack[s->nextEmpty] = n;
    (s->nextEmpty)++;
}

int Pop (Stack s) {
    (s->nextEmpty)--;
    return s->stack[s->nextEmpty];
}
