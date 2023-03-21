#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX 10
#define UPPER 100000
#define ROWS 4
#define COLS 4

struct idList
{
    pthread_t thID;
    struct idList *next;
};

long counter = 0;  /* variable to be incremented by each thread */
sem_t mysemaphore; /* global semaphore */

/* This function specifies the body of each thread */

void *some_thread(void *param) /* routine should do counter += UPPER */
{                              /* if the semaphores work! */
    int j, temp;
    int id = *(int *)param;

    free(param);
    for (j = 0; j < UPPER; j++)
    {

        //    sem_wait( &mysemaphore );  	/*  semaphore WAIT operation */
        // The wait systems call waits on mysemaphore until it becomes non-zero.
        // Then, it atomically decreases mysemaphore.
        temp = 2 * counter; // Start of the critical section
        temp = temp + 2;
        counter = temp / 2;
        // 	sem_post( &mysemaphore );   	/*  semaphore SIGNAL operation */
    }
    pthread_exit(NULL);
}

int main()
{
    int numbers[ROWS][COLS];

    int x, y;

    // Read integers into a 2D array
    for (x = 0; x < ROWS; x++)
    {
        for (y = 0; y < COLS; y++)
        {
            if (scanf("%d", &numbers[x][y]) != 1)
            {
                printf("\nCould not read integer.\n");
                return 1;
            }
        }
    }

    // Print the original array
    printf("Original Array:\n");
    for (x = 0; x < ROWS; x++)
    {
        for (y = 0; y < COLS; y++)
        {
            printf("%d ", numbers[x][y]);
        }
        printf("\n");
    }
    printf("\n");

    // Print the sorted array
    printf("Sorted Array:\n ");
    for (i = 0; i < ROWS; i++)
    {
        for (y = 0; y < COLS; y++)
        {
            printf("%d ", msg.numbers[i][y]);
        }
        printf("\n");
    }
}