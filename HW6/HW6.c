// you can run code by this command:
// gcc -o OS_HW6_99102401_AliNazari_Practical.o OS_HW6_99102401_AliNazari_Practical.c -lpthread -lrt -lm
// then ./OS_HW6_99102401_AliNazari_Practical.o
// after a while, kill the code with ctrl + c
// I don't know why, but my code won't finish by itself
// so you should kill it
// but the code create output files and write results in them
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
int car_path[100][100];
int car_p[100];
char car_name[100];
char road_source[100];
char road_destination[100];
int road_hardness[100];
int road_is_full[100];
sem_t road_mutex[100];
int threadNumber = 2;
long long int total_emission = 0;
long long int emission = 0;
void init_arrays(){
    for(int i = 0; i < 100; i++){
        sem_init(&road_mutex[i], 0, 1);
        for(int j = 0; j < 100; j++){
            car_path[i][j] = -1;
        }
    }
}
void* thread_function(void* arg){
    int *main_array_int_mode = (int *) arg;
    int number = main_array_int_mode[0];
    int index = 0;
    while(1){
        int* car_path_index = car_path[number];
        sem_wait(&road_mutex[car_path_index[index]]);
        int car_road = car_path_index[index];
        if(car_road == -1)
            break;
        if(road_is_full[car_road] == 0){
            road_is_full[car_road] = 1;
            time_t enter_time = time(NULL);
            for(int i_def = 0; i_def <= 1e7; i_def++)
                emission += floor(i_def / (1e6 * car_p[number] * road_hardness[car_path_index[index]]));
            total_emission += emission;
            road_is_full[car_road] = 0;
            time_t exit_time = time(NULL);
            char buffer[512];
            sprintf(buffer, "./%d-%d.txt", car_road, number);
            FILE* file;
            file = fopen(buffer, "w");
            fprintf(file, "Entrance Node Name: %c, Entrance Time: %ld, Exit Node Name: %c, Exit Time: %ld, Emission: %lld, Total Emisiion: %lld", road_source[car_path_index[index]], enter_time, road_destination[car_path_index[index]], exit_time, emission, total_emission);
            emission = 0;
            fclose(file);
        }
        sem_post(&road_mutex[car_path_index[index]]);
        index += 1;
    }
}
int main(void){
    // source: https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
    // source: https://stackoverflow.com/questions/22621952/convert-char-to-string-in-c
    // source: https://www.programiz.com/c-programming/library-function/string.h/strcmp
    // source: https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
    // source: https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    srand(time(0));
    init_arrays();
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen("input.txt", "r");
    int counter = 0;
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        char str1[2] = {line[0], '\0'};
        if (strcmp(str1, "#") == 0)
            break;
        char* token = strtok(line, " - ");
        while( token != NULL ) {
            if (counter == 0){
                road_source[i] = token[0];
            }
            else if (counter == 1){
                road_destination[i] = token[0];
            }
            else{
                road_hardness[i] = atoi(token);
                i += 1;
            }
            counter += 1;
            counter %= 3;
            token = strtok(NULL, " - ");
        }
    }
    fclose(fp);
    FILE * fp2;
    char * line2 = NULL;
    size_t len2 = 0;
    ssize_t read2;
    fp2 = fopen("input.txt", "r");
    int flag = 0;
    int counter2 = 0;
    int i2 = 0;
    while ((read2 = getline(&line2, &len2, fp2)) != -1) {
        if (flag == 1){
            if (counter2 == 0){
                char string[100];
                strcpy(string, line2);
                char* token = strtok(string, " - ");
                char first_char = token[0];
                token = strtok(NULL, " - ");
                int the_path = 0;
                while(token != NULL){
                    char second_char = token[0];
                    for(int j = 0; j < 20; j++){
                        if(road_source[j] == first_char && road_destination[j] == second_char){
                            car_path[i2][the_path] = j;
                            the_path += 1;
                            break;
                        }
                    }
                    first_char = second_char;
                    token = strtok(NULL, " - ");
                }
            }
            else{
                int count = atoi(line2);
                car_p[i2] = rand()%10 + 1;
                for(int j = 1; j < count; j++){
                    car_p[i2+1] = rand()%10 + 1;
                    memcpy(car_path[i2+1], car_path[i2], 200);
                    car_name[i2+1] = car_name[i2];
                    i2++;
                    threadNumber += 1;
                }
                i2 += 1;
            }
            counter2 += 1;
            counter2 %= 2;
        }
        char str1[2] = {line2[0], '\0'};
        if (strcmp(str1, "#") == 0){
            flag = 1;
        }
    }
    fclose(fp2);
    pthread_t thread_id[threadNumber];
    for(int i = 0; i < threadNumber; i++){
        int (*to_pass) = malloc(4);
        to_pass[0] = i;
        pthread_create(&thread_id[i], NULL, thread_function, (void*)to_pass);
    }
    for(int i = 0; i < threadNumber; i++){
        pthread_join(thread_id[i], NULL);
    }
    return 0;
}