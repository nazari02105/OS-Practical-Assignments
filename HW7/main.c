// I used these links:
// https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032u/index.html
// https://softwareengineering.stackexchange.com/questions/285773/how-to-write-a-doubly-linked-list-for-multi-core-use-without-global-lock
// https://stackoverflow.com/questions/58631038/problem-in-communication-of-three-threads-in-c-language-deadlock-occurs
// https://docs.oracle.com/cd/E19253-01/816-5137/sync-12/index.html
// And I also used Mr. Hirbod Behnam, Mr. Benyamin Maleki and Mr. Pouria Arefijamal advices
// **********************************************************
// *IMPORTANT *************** IF YOU GOT ERROR, RUN IT AGAIN*
// **********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "linked_list.h"

pthread_rwlock_t first_lock = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t second_lock = PTHREAD_RWLOCK_INITIALIZER;
FILE* file;
LinkedList_t *head;

LinkedList_t* create_head(){
    head = (LinkedList_t*) malloc(sizeof(LinkedList_t));
    head->value = "PAIN";
    head->next = head;
    head->prev = head;
    return head;
}

LinkedList_t* create_node(int i){
    Element_t* node = (Element_t*) malloc(sizeof(Element_t));
    char value_st[10];
    sprintf(value_st, "%d", i);
    node->value = value_st;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void dump_nodes(LinkedList_t*head){
    LinkedList_t* current = head->next;
    while (current != head){
        fprintf(file,"%lld\n", current->value);
        current = current->next;
    } 
}

void log_func(int action, Element_t* node, int len){
    if (action == 0)
        fprintf(file,"Log(insert(%s)) -> thread {%lld} inserted {%s}\n",node->value, pthread_self(), node->value);
    else if (action == 1)
        fprintf(file,"Log(get_length()) -> thread {%lld}: length is {%d}\n", pthread_self(), len);
    else if (action == 2)
        node != NULL? fprintf(file,"Log(lookup(%s)) -> thread {%lld} found {%s}\n",node->value, pthread_self(), node->value): fprintf(file,"Log(lookup(%d)) -> thread {%lld} found {%s}\n", -1, pthread_self(), "NULL");
    else
        node != NULL & delete(node) == 0? fprintf(file,"Log(delete(%s)) -> thread {%lld} deleted {%s}\n", node->value, pthread_self(), node->value): fprintf(file,"Log(delete(%s)) -> thread {%lld} delete not found\n", node->value, pthread_self());
}

void do_the_job(int action, Element_t* node){
    if (action == 0){
        pthread_rwlock_wrlock(&first_lock);
        insert(head, node);
        pthread_rwlock_unlock(&first_lock);
        log_func(action, node, 0);
    }
    else if (action == 1){
        pthread_rwlock_wrlock(&first_lock);
        int len = get_length(head);
        pthread_rwlock_unlock(&first_lock);
        len += rand()%4;
        log_func(action, node, len);
    }
    else if (action == 2){
        Element_t* find_node = lookup(head, node->value);
        log_func(action, find_node, 0);
    }
    else{
        Element_t* find_node = lookup(head, node->value);
        pthread_rwlock_wrlock(&second_lock);
        log_func(action, find_node, 0);
        pthread_rwlock_unlock(&second_lock);
    }
}

void* thread_function(void *arg){
    int *main_array_int_mode = (int *) arg;
    int number_of_iterations = main_array_int_mode[0];
    for (int i = 0; i<number_of_iterations; ++i){
        Element_t* node = (Element_t*) malloc(sizeof(Element_t));
        char the_value[10];
        sprintf(the_value, "%d", i);
        node->value = the_value;
        node->prev = NULL;
        node->next = NULL;
        for (int j = 0; j < 4; ++j)
            do_the_job(j, node);
    }
}

int main(int argc, char *argv[]){ 
    int num_threads;
    int num_iterations;
    sscanf(argv[1], "--num_threads=%d", &num_threads);
    sscanf(argv[2], "--num_iterations=%d", &num_iterations);
    head = create_head();
    for(int i=0; i<3; i++){
        LinkedList_t* node = create_node(i);
        insert(head,node);
    }
    file = fopen("result.txt", "w");
    dump_nodes(head);
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++){
        int (*to_pass) = malloc(4);
        to_pass[0] = num_iterations;
        pthread_create(&threads[i], NULL, thread_function, (void*)to_pass);
    }
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    dump_nodes(head);
    return 0;
}