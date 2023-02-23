#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int all_entries[10000];
int entries_first_number[1000];
int entries_second_number[1000];
int entries_first_number2[1000];
int entries_second_number2[1000];
pthread_t all_threads[4];
int threads_finished[4] = {0,0,0,0};
int threads_first_number[4] = {0,0,0,0};
int threads_second_number[4] = {0,0,0,0};
int threads_started[4] = {1,1,1,1};
int algorithm(int first, int second){
    if(first==0) return second+1;
    else if(second==0) return algorithm(first-1,1);
    else return algorithm(first-1,algorithm(first,second-1));
}
void* thread_function(void *input_array){
    int *converted_array = (int *) input_array;
    do if(threads_started[converted_array[0]] == 0)
        if(all_entries[(threads_first_number[converted_array[0]] * threads_second_number[converted_array[0]]) + (threads_first_number[converted_array[0]] + threads_second_number[converted_array[0]])] == 0){
            all_entries[(threads_first_number[converted_array[0]] * threads_second_number[converted_array[0]]) + (threads_first_number[converted_array[0]] + threads_second_number[converted_array[0]])] = algorithm(threads_first_number[converted_array[0]],threads_second_number[converted_array[0]]);;
            threads_started[converted_array[0]] = 1;
        }
    while (threads_finished[converted_array[0]] == 0);
}
int main(){
    int total_inputs;
    printf("total entries: ");
    scanf(" %d",&total_inputs);
    for(int i=0;i<total_inputs;i++) scanf(" %d %d",&entries_first_number[i],&entries_second_number[i]);
    int main_index = 0;
    for (int j = 0; j < total_inputs; j++){
        int min_value = 100000;
        int min_index = -1;
        for (int i = 0; i < total_inputs; i++){
            int sum = entries_first_number[i] + entries_second_number[i];
            if (sum < min_value){
                min_value = sum;
                min_index = i;
            }
        }
        entries_first_number2[main_index] = entries_first_number[min_index];
        entries_second_number2[main_index] = entries_second_number[min_index];
        entries_first_number[min_index] = 100000;
        entries_second_number[min_index] = 100000;
        main_index += 1;
    }
    for(int i=0;i<4;i++){
        int (*to_pass) = malloc(4); 
        to_pass[0] = i;
        pthread_create(&all_threads[i],NULL,thread_function,(void *)to_pass);
    }
    for(int i=0;i<total_inputs;i++){
        int trust = 0;
        while(trust == 0)
            for(int j=0;j<4;j++)
                if(threads_started[j] == 1 && threads_finished[j] == 0){
                    threads_started[j] = 0;
                    threads_first_number[j] = entries_first_number2[i];
                    threads_second_number[j] = entries_second_number2[i];
                    trust++;
                    break;
                }
    }
    for(int j=0;j<4;j++) threads_finished[j]=1;
    for(int i=0;i<4;i++) pthread_join(all_threads[i],NULL);
    for(int i=0;i<total_inputs;i++) printf("A(%d,%d) = %d\n",entries_first_number2[i],entries_second_number2[i],all_entries[(entries_first_number2[i] * entries_second_number2[i]) + (entries_first_number2[i] + entries_second_number2[i])]);
    return 0;
}