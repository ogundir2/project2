#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 4

// Global variables
int array[N][N];
sem_t semaphores[N];
pthread_t threads[N];

// Function to read input and store it in the 2D array
void read_input()
{
    printf("Enter %d integers: \n", N * N);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            scanf("%d", &array[i][j]);
        }
    }
}

// Function to print the array
void print_array()
{
    printf("\n");
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to perform Bubblesort
void bubblesort(int *arr, int len, int ascending)
{
    for (int i = 0; i < len - 1; ++i)
    {
        for (int j = 0; j < len - 1 - i; ++j)
        {
            if (ascending ? arr[j] > arr[j + 1] : arr[j] < arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to perform Shearsort
void *shearsort(void *arg)
{
    int id = *(int *)arg;
    free(arg);

    for (int phase = 0; phase < 2 * N; ++phase)
    {
        sem_wait(&semaphores[id]);

        if (phase % 2 == 0)
        { // Sort row
            bubblesort(array[id], N, id % 2 == 0);
        }
        else
        { // Sort column
            int col[N];
            for (int i = 0; i < N; ++i)
            {
                col[i] = array[i][id];
            }
            bubblesort(col, N, 1);
            for (int i = 0; i < N; ++i)
            {
                array[i][id] = col[i];
            }
        }

        sem_post(&semaphores[(id + 1) % N]);
    }

    pthread_exit(NULL);
}

int main()
{
    read_input();
    printf("Initial array:\n");
    print_array();

    for (int i = 0; i < N; ++i)
    {
        sem_init(&semaphores[i], 0, i == 0 ? 1 : 0);
    }

    for (int i = 0; i < N; ++i)
    {
        int *arg = (int *)malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, shearsort, arg);
    }

    for (int i = 0; i < N; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Sorted array:\n");
    print_array();

    for (int i = 0; i < N; ++i)
    {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}
