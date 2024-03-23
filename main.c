#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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

struct ThreadInfo {
    int count_to_value;
    int progress;

    pthread_t current_thread;
};

void build_progress_bar(struct ThreadInfo *thread_info, char *process_name) {
    int number_of_chars = (thread_info->progress * 100 / thread_info->count_to_value)
                           * PROGRESS_BAR_LENGTH / 100;

    printf("\r%s[", process_name);

    for(int i = 0; i < number_of_chars; i++) {
        //if (i % 2 == 0)
        //    printf("\033[0;32m"); // Dark Green
        //else if (i % 2 == 1)
        //    printf("\033[1;32m"); // Light Green

        //printf("∎");
        printf("▰");
    }
    if(thread_info->progress < thread_info->count_to_value) {
        printf(BGRN); // Bright Green
        printf("▰");
        printf(RESET);
    }
    for(int i = 0; i < PROGRESS_BAR_LENGTH - number_of_chars - 1; i++) {
        printf(" ");
    }
    printf("]" TICK "\n");
}

void *my_thread_func(void *arg) {
    struct ThreadInfo *thread_info = arg;    

    for(thread_info->progress = 0;
        thread_info->progress <
        thread_info->count_to_value;
        thread_info->progress++) {

        usleep(1000);
    }

    return NULL;
}

int main() {
    struct ThreadInfo threads[NUMBER_OF_THREADS];

    bool done = false;

    for(int i  = 0; i < NUMBER_OF_THREADS; i++) {
        threads[i].count_to_value = rand() % 10000;
        threads[i].progress = 0;

        pthread_create(&threads[i].current_thread, NULL, my_thread_func, &threads[i]);
    }
    while(!done) {
        done = true;

        for(int i = 0; i < NUMBER_OF_THREADS; i++) {
            build_progress_bar(&threads[i], "BUILDING FILES ");

            if(threads[i].progress < threads[i].count_to_value) {
                done = false;
            }
        }
        if(!done) {
            printf(ESC CSI "%d" PREVIOUS_LINE, NUMBER_OF_THREADS); // ESC sequence
        }
        usleep(10000);
    }
    return 0;
}
