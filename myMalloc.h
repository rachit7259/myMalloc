#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

typedef struct blockMeta
{
    size_t size;
    struct blockMeta* next;
    int free;
    int magic; //for debugging
}blockMeta;

#define META_SIZE sizeof(struct blockMeta)

struct blockMeta* requestMemory (struct blockMeta* prev, size_t size);
struct blockMeta* findFreeBlock (struct blockMeta** prev, size_t size);

void* myMalloc (size_t size);
void myFree (void* ptr);
struct blockMeta *getBlockPtr(void *ptr);

void *myRealloc(void *ptr, size_t size);
void *myCalloc(size_t numEle, size_t eleSize);