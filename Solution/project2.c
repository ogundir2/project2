#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX 10
#define UPPER 100000
#define N 4

struct idList
{
    pthread_t thID;
    struct idList *next;
};

int numbers[N][N];     // 2D array of integers
sem_t mysemaphores[N]; // Global semaphores

// Bubble sort a 1D array
void bubble_sort(int *arr, bool ascending)
{
    int num, next, i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N - i - 1; j++)
        {
            num = arr[j];
            next = arr[j + 1];
            if (ascending == true ? num > next : num < next)
            {
                arr[j] = next;
                arr[j + 1] = num;
            }
        }
    }
}

// Shear sort a 2D array
void *shear_sort(void *param)
{
    int p;
    int id = *(int *)param;
    free(param);

    // For each phase
    for (p = 0; p < 2 * N; p++)
    {
        sem_wait(&semaphores[id]);

        if (p % 2 == 0)
        {
            // Sort a row
            bubble_sort(numbers[id], id % 2 == 0 ? true : false);
        }
        else
        {
            // Sort a column
            int col[N];
            for (int i = 0; i < N; ++i)
            {
                col[i] = numbers[i][id];
            }
            bubble_sort(col, true);
            for (int i = 0; i < N; ++i)
            {
                numbers[i][id] = col[i];
            }
        }

        sem_post(&semaphores[(id + 1) % N]);
    }

    pthread_exit(NULL);
}

int main()
{
    int i, j;
    int *param;

    // Read integers into a 2D array
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (scanf("%d", &numbers[i][j]) != 1)
            {
                printf("\nCould not read integer.\n");
                return 1;
            }
        }
    }

    // Print the original array
    printf("Original Array:\n");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", numbers[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    pthread_t *threadID; // The ID of the newly created thread

    for (i = 0; i < N; i++)
    {
        int value = 0;
        if (i == 0)
        {
            value = 1;
        }

        sem_init(&mysemaphores[i], 0, value);
    }

    struct idList *root; // Points to a linked list of thread IDs
    struct idList *currNode, *temp;

    // Create n identical threads
    for (i = 0; i < N; i++)
    {
        param = (int *)malloc(sizeof(int)); /* each thread requires a */
        *param = i;                         /* SEPARATE copy of its parameter */

        temp = (struct idList *)malloc(sizeof(struct idList)); // Allocate some memory for a new node
        if (!i)
        {
            root = temp;
            currNode = temp;
        } // initialize the root of the linked list

        threadID = (pthread_t *)malloc(sizeof(pthread_t));

        int thCrEr = pthread_create(threadID, NULL, shear_sort, param);

        if (thCrEr)
            printf("Error in creating a new thread!!!");

        temp->thID = *threadID;
        temp->next = NULL;
        currNode->next = temp;
        currNode = temp;

        printf("A new thread created with ID %d \n", *threadID);
    }

    currNode = root;
    /* Wait for all the threads to exit */

    for (i = 0; i < n; i++)
    {
        printf("\n Joining thread: %d", currNode->thID);
        pthread_join(currNode->thID, NULL);

        currNode = currNode->next;
    }

    // Print the sorted array
    printf("Sorted Array:\n");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", numbers[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    exit(0);
}