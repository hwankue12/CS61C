#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ll.h"

struct list {
    char *value;
    struct list *next;
    struct list *prev;
};

const char *list_node_value(list_t *node) {
    return node->value;
}

list_t *list_first(list_t *list) {
    return list;
}

list_t *list_last(list_t *list) {
    return list->prev;
}

list_t *list_next(list_t *node) {
    return node->next;
}

list_t *list_previous(list_t *node) {
    return node->prev;
}

static void failed_allocation(void) {
    fprintf(stderr, "Out of memory.\n");
    abort();
}

static list_t *new_node(const char *value) {
    list_t *node = malloc(sizeof(list_t));
    if (!node) failed_allocation();
    node->value = malloc((strlen(value)+1)*sizeof(char));// This is one change

    if (!node->value) failed_allocation();
    strcpy(node->value, value);
    node->value[strlen(value)]=0;
    node->next=node;   //initialize the node so that it points to self
    node->prev=node;
    return node;
}

list_t *list_insert_before(list_t *list, list_t *node, const char *value) {
    if(list){
        list_t *insert_node = new_node(value);
        insert_node->prev = node->prev;
        insert_node->next = node;
        insert_node->next->prev = insert_node;
        insert_node->prev->next = insert_node;
        if (list == node) {
            return insert_node;
        } else {
            return list;
        }
    } else{ // to check for NULL list point passed in
      list_t* insert_node=new_node(value); 
            return insert_node;
    }
}

list_t *list_append(list_t *list, const char *value) {
    if (list) {
        (void) list_insert_before(list, list, value);
        return list;
    } else {
        list_t *node = new_node(value);
        node->prev = node->next = node;
        return node;
    }
}

list_t *list_prepend(list_t *list, const char *value) {
    if (list) {
        return list_insert_before(list, list, value);
    } else {
        list_t *node = new_node(value);
        node->prev = node->next = node;
        return node;
    }
}

list_t *list_remove(list_t *list, list_t *node) {
 
  if (list != node) { 
    node->prev->next = node->next;
    node->next->prev = node->prev;
    
    free(node->value);// This is a place for change
    free(node);
   
        return list;
    } else {     // This part is rewritten to free node
        if( list->next == list ){// deal with list with only one node
          free(node->value);
          free(node);
          return NULL;
        }
        else {
          node->prev->next=node->next;
          node->next->prev=node->prev;
          list_t *temp=node->next;
          free(node->value);
          free(node);
          return temp;
        }
    }
}

void list_free(list_t *list) {
    if (list) {
        while (list_remove(list, list_last(list)) != NULL) {}
    }
}

void list_foreach(list_t *list, void (*function)(const char*)) {
    if (list) {
        list_t *cur = list_first(list);
        do {
            function(cur->value);
            cur = cur->next;
        } while (cur != list_first(list));
    }
}
