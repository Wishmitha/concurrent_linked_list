#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include "timer.h"
#include "linkedList.h"


long thread_count = 0;
pthread_mutex_t mutex;
struct node *head = NULL;

int n, m;
float m_percentage_of_Member, m_percentage_of_Insert, m_percentage_of_Delete;

void GetArguments(int argc, char *argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    n = (int) strtol(argv[2], (char **) NULL, 10);
    m = (int) strtol(argv[3], (char **) NULL, 10);

    m_percentage_of_Member = (float) atof(argv[4]);
    m_percentage_of_Insert = (float) atof(argv[5]);
    m_percentage_of_Delete = (float) atof(argv[6]);

//    printf("%ld, %d, %d, %.6f, %.6f, %.6f\n", thread_count, n, m, m_percentage_of_Member, m_percentage_of_Insert, m_percentage_of_Delete);
}

void *pthread_on_linked_list(void *rank) {
    long long i;
    long long my_m = m / thread_count;

    for (i = 0; i < my_m; i++) {
        float prob = (rand() % 10000 / 10000.0);

        int r = rand() % 65536;

        if (prob < m_percentage_of_Member) {
            pthread_mutex_lock(&mutex);
            Member(r, head);
            pthread_mutex_unlock(&mutex);

        } else if (prob < m_percentage_of_Member + m_percentage_of_Insert) {
            pthread_mutex_lock(&mutex);
            Insert(r, &head);
            pthread_mutex_unlock(&mutex);

        } else {
            pthread_mutex_lock(&mutex);
            Delete(r, &head);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}


float linkedList_mutex_program(int argc, char *argv[]) {
    int i = 0;
    long thread;
    pthread_t *thread_handles;
    double_t start, end;
    GetArguments(argc, argv);

    head = NULL;

    for (; i < n; i++) {
        int r = rand() % 65536;
        if (!Insert(r, &head)) {
            i--;
        }
    }

    thread_handles = (pthread_t *) malloc(thread_count * sizeof(pthread_t));


    GET_TIME(start);

    pthread_mutex_init(&mutex, NULL);

    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, pthread_on_linked_list, (void *) thread);
    }

    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_mutex_destroy(&mutex);

    GET_TIME(end);

    return (end - start);

}

int main(int argc, char *argv[]) {
    const int samples = 4;

    for (int j = 0; j < samples; j++) {
        pthread_mutex_init(&mutex, NULL);
        float time = linkedList_mutex_program(argc, argv);
        pthread_mutex_destroy(&mutex);
        printf("%.6f  \n", time);
    }
    return 0;
}