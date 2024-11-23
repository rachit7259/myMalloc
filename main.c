#include "myMalloc.h"

void main()
{
    int *arr, *arr2, *arr3, *arr4;

    arr = (int*) myMalloc(10*sizeof(int));

    if(!arr)
        printf("Memory Error!\n");

    printf("Address of arr: %p\n", arr);

    for(int i = 0; i < 10; i++)
    {
        arr[i] = i;
    }

    for(int i = 0; i < 10; i++)
    {
        printf(" %d", arr[i]);
    }
    printf("\n");

    arr2 = (int*) myRealloc ((void*) arr, 15*sizeof(int));

    if(!arr2)
        printf("Memory Error!\n");

    printf("Address of arr2: %p\n", arr2);

    for(int i = 0; i < 15; i++)
    {
        printf(" %d", arr2[i]);
    }
    printf("\n");

    arr3 = (int*) myRealloc ((void*) arr2, 10*sizeof(int));

    if(!arr3)
        printf("Memory Error!\n");

    printf("Address of arr3: %p\n", arr3);

    for(int i = 0; i < 10; i++)
    {
        printf(" %d", arr3[i]);
    }
    printf("\n");

    arr4 = (int*) myCalloc (10, sizeof(int));

    if(!arr4)
        printf("Memory Error!\n");

    printf("Address of arr4: %p\n", arr4);

    for(int i = 0; i < 10; i++)
    {
        printf(" %d", arr4[i]);
    }
    printf("\n");

    for(int i = 0; i < 10; i++)
    {
        arr4[i] = i;
    }

    for(int i = 0; i < 10; i++)
    {
        printf(" %d", arr4[i]);
    }
    printf("\n");

    myFree(arr3);
    myFree(arr4);
}