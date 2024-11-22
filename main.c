#include "myMalloc.h"

void main()
{
    int* arr;

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

    myFree(arr);
}