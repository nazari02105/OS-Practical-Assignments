// if functions, we don't know that they give us head or not
// so I use circular linked list to handle this problem
// I used this link a lot to understand circular linked list
// https://www.geeksforgeeks.org/program-for-all-operations-on-circular-linked-list-in-c/

#include <string.h>
#include "linked_list.h"

void insert(LinkedList_t *list, Element_t *element){
    element->next = list->next;
    list->next->prev = element;
    list->next = element;
    element->prev = list;
}

int delete( Element_t *element){
    Element_t* next = element->next;
    Element_t* prev = element->prev;
    if(prev == NULL || next == NULL)
        return 1;
    next->prev = prev;
    prev->next = next;
    return 0;
}

Element_t *lookup(LinkedList_t *list, const char *value){
    LinkedList_t * current = list;
    if(strcmp(value, (current->value)) == 0)
        return current;
    current = current->next;
    while(current != list){
        if(strcmp(value, (current->value)) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

int get_length(LinkedList_t *list){
    int len = 0;
    LinkedList_t* current = list->next;
    while(current != list){
        current = current->next;
        len++;
    }
    return len;
}