#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define PROGRESS_BAR_LENGTH 30
#define NUMBER_OF_THREADS 5

#define ESC "\033"
#define CSI "["
#define PREVIOUS_LINE "F"
#define BACK_SPACE "D"

#define TICK "✓"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define BGRN  "\033[0;92m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

struct BarThreadInfo {
    int count_to_value;
    int progress;

    pthread_t current_thread;
};

void build_progress_bar(struct BarThreadInfo *thread_info, char *process_name) {
    int number_of_chars = (thread_info->progress * 100 / thread_info->count_to_value)
                           * PROGRESS_BAR_LENGTH / 100;

    printf("\r%s[", process_name);

    for(int i = 0; i < number_of_chars; i++) {
        printf("▰");
    }
    if(thread_info->progress < thread_info->count_to_value) {
        printf(BGRN);
        printf("▰");
        printf(RESET);
    }
    for(int i = 0; i < PROGRESS_BAR_LENGTH - number_of_chars - 1; i++) {
        printf(" ");
    }
    printf("]");
    printf(BGRN);
    printf(TICK "\n");
    printf(RESET);
    
    return;
}

void *progress_thread_setup(void *arg) {
    struct BarThreadInfo *thread_info = arg;    

    for(thread_info->progress = 0;
        thread_info->progress <
        thread_info->count_to_value;
        thread_info->progress++) {

        usleep(thread_info->progress);
    }

    return NULL;
}

void *progress_thread_setup_2(void *arg) {
    struct BarThreadInfo *thread_info = arg;    

    for(thread_info->progress = 0;
        thread_info->progress <
        thread_info->count_to_value;
        thread_info->progress++) {

        usleep(1000);
    }

    return NULL;
}

void process_progress_bar(int bar_threads, char *process_name, struct BarThreadInfo threads[]) {
    bool done = false;

    while(!done) {
        done = true;

        for(int i = 0; i < bar_threads; i++) {
            build_progress_bar(&threads[i], process_name);

            if(threads[i].progress < threads[i].count_to_value) {
                done = false;
            }
        }
        if(!done) {
            printf(ESC CSI "%d" PREVIOUS_LINE, bar_threads);
        }
        usleep(10000);
    }
}

void MAKE_CBAR(int bar_threads, int process_value, char *process_name) {
    struct BarThreadInfo threads[bar_threads];

    for(int i  = 0; i < bar_threads; i++) {
        threads[i].count_to_value = process_value;
        threads[i].progress = 0;

        pthread_create(&threads[i].current_thread, NULL, progress_thread_setup, &threads[i]);
    }
    process_progress_bar(bar_threads, process_name, threads);
}

int add(int a, int b) {
    return a + b;
}


float divide(int a, int b) {
    return (float)a / (float)b;
}

void plus_ele(int arr[]) {
    for(int i = 0; i < 10; i++) {
        printf("%d", arr[i] + i);
    }
}

int main() {
    // Give an array of function pointers to the params of MAKE_CBAR(... function_array[]) func, then 

    //MAKE_CBAR(2, 1000, "testing: \t");
    MAKE_CBAR(1, 1000 * 2, "testing_2: \t");

    double time_taken;

    int arr[] = {1, 4, 2, 5, 6, 8, 8, 5, 9};

    clock_t t; 

    t = clock(); 
    plus_ele(arr);
    t = clock() - t; 

    time_taken = ((double)t) / CLOCKS_PER_SEC;
    
    printf("%d", (int)time_taken);

    MAKE_CBAR(1, 0, "ele");

    return 0;
}
