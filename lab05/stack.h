/*********************************************************************
stack.h

This file implements a very simple stack data structure that will be
used to demonstrate linker and object file features.

**********************************************************************/

/* make the struct stack a type */
typedef struct stack *Stack;

/* Function Prototypes */
Stack EmptyStack ();

int IsEmpty (Stack s);

void Push (Stack s, int n);

int Pop (Stack s);
