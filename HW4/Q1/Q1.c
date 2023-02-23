#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
pthread_barrier_t threads_index;
char * file_name = "./input.txt";
char * line = NULL;
size_t len = 0;
int algorithm[100][100][100] = {INT_MAX / 2};
FILE * fp;
ssize_t read;
void* thread_function(void* main_array){
    int *main_array_int_mode = (int *) main_array;
    for(int i=1;i<main_array_int_mode[2];i++){
        algorithm[main_array_int_mode[0]][main_array_int_mode[1]][i] = algorithm[main_array_int_mode[0]][main_array_int_mode[1]][i-1];
        pthread_barrier_wait(&threads_index);
        for(int j=1;j<main_array_int_mode[2];j++) if (algorithm[main_array_int_mode[0]][main_array_int_mode[1]][i] > algorithm[main_array_int_mode[0]][j][i-1] + algorithm[j][main_array_int_mode[1]][0]) algorithm[main_array_int_mode[0]][main_array_int_mode[1]][i] = algorithm[main_array_int_mode[0]][j][i-1] + algorithm[j][main_array_int_mode[1]][0];
     }
}
pthread_t all_threads[100][100];
int main(){
    int number_of_dimension = 0;
    fp = fopen(file_name, "r");
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        number_of_dimension += 1;
        char * token = strtok(line, " ");
        int j = 0;
        while( token != NULL ) {
            if (strcmp(token, "∞") == 0 || strcmp(token, "∞\n") == 0) algorithm[i][j][0] = INT_MAX / 2;
            else algorithm[i][j][0] = atoi(token);
            token = strtok(NULL, " ");
            j += 1;
        }
        i += 1;
    }
    fclose(fp);
    if (line) free(line);
    pthread_barrier_init(&threads_index, NULL, number_of_dimension*(number_of_dimension-1));
    for(int i=0;i<number_of_dimension;i++) for(int j=0;j<number_of_dimension;j++)
            if (i != j){
                // thanks to http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/9-PThread/Pass.html
                int (*to_pass) = malloc(12); 
                to_pass[0] = i;
                to_pass[1] = j;
                to_pass[2] = number_of_dimension;
                pthread_create(&all_threads[i][j],NULL,thread_function,(void*)to_pass);
            }
    for(int i=0;i<number_of_dimension;i++) for(int j=0;j<number_of_dimension;j++) if (i != j) pthread_join(all_threads[i][j],NULL);
    // now we print result
    for(int i=0;i<number_of_dimension-1;i++){
        for(int j=0;j<number_of_dimension-1;j++){
            if(algorithm[i][j][number_of_dimension-1] == INT_MAX / 2 || i==j) printf("∞ ");
            else printf("%d ",algorithm[i][j][number_of_dimension-1]);
        }
        printf("\n");
    }
    return 0;
}