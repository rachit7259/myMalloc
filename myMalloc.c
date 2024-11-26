#include "myMalloc.h"

void* globalBase = NULL;

struct blockMeta* requestMemory (struct blockMeta* prev, size_t size)
{
    struct blockMeta* block;

    block = sbrk(0); //current top of the heap

    void* req = sbrk(size + META_SIZE); //increments heap size

    assert((void*)block == req); //not thread safe

    if(req == (void*) -1)
        return NULL;
    
    if(prev)
        prev->next = block;

    block->size = size;
    block->next = NULL;
    block->free = 0;
    block->magic = 0x12345678;
    
    return block;
}

struct blockMeta* findFreeBlock (struct blockMeta** prev, size_t size)
{
    struct blockMeta* current = globalBase;

    while(current && !(current->free && current->size >= size))
    {
        *prev = current;
        current = current->next;
    }

    return current;
}

void splitBlock(struct blockMeta* block, size_t reqSize)
{
    struct blockMeta* newBlock;
    char* shift;

    shift = (char*) (block + 1);
    shift += reqSize;

    newBlock = (struct blockMeta*) shift;
    newBlock->size = block->size - reqSize;
    newBlock->next = block->next;
    newBlock->free = 1;
    newBlock->magic = 0x55555555;

    block->next = newBlock;
    block->size = reqSize;

    return;
}

void mergeBlock(struct blockMeta* block)
{
    struct blockMeta* neighbor;

    neighbor = block->next;

    if(!neighbor)
        return;

    if(neighbor->free)
    {
        block->next = neighbor->next;
        block->size += neighbor->size + META_SIZE;

        memset(neighbor, 0, META_SIZE);

        printf("Block merged\n");
    }

    return;
}

void* myMalloc (size_t size)
{
    struct blockMeta* block;

    if(size <= 0)
        return NULL;

    if (!globalBase)
    {
        block = requestMemory(NULL, size);

        if(!block)
            return NULL;

        globalBase = block;
    }
    else
    {
        struct blockMeta* prev = globalBase;

        block = findFreeBlock(&prev, size);

        if(!block)
        {
            block = requestMemory(prev, size);

            if(!block)
                return NULL;
        }
        else
        {
            //found a free block
            //TODO: split block if required

            if(block->size > size)
                splitBlock(block, size);

            block->free = 0;
            block->magic = 0x77777777;
        }
    }
    
    return (block+1);
}

struct blockMeta *getBlockPtr(void *ptr) {
  return (struct blockMeta*)ptr - 1;
}

void myFree (void* ptr)
{
    if(!ptr)
        return;

    struct blockMeta* blockPtr = getBlockPtr(ptr);

    assert(blockPtr->free == 0);
    assert(blockPtr->magic == 0x77777777 || blockPtr->magic == 0x12345678);

    //TODO: merge blocks if necessary
    mergeBlock(blockPtr);

    blockPtr->free = 1;
    blockPtr->magic = 0x55555555;

    return;
}

void *myRealloc(void *ptr, size_t size)
{
    if(!ptr)
    {
        return myMalloc(size);
    }

    struct blockMeta* blockPtr = getBlockPtr(ptr);

    if(blockPtr->size >= size)
    {
        //split the block
        splitBlock(blockPtr, size);
        return ptr;
    }

    //malloc new memory and free old memory
    //copy data to the new space

    void* newPtr;

    newPtr = myMalloc(size);

    if(!newPtr)
        return NULL;
    
    memcpy(newPtr, ptr, blockPtr->size);

    myFree(ptr);

    return newPtr;
}

void *myCalloc(size_t numEle, size_t eleSize)
{
    if (numEle > SIZE_MAX / eleSize) {
        // Handle potential overflow
        fprintf(stderr, "calloc: Multiplication overflow detected!\n");
        return NULL;
    }

    size_t size = numEle * eleSize;

    void* ptr = myMalloc(size);

    memset(ptr, 0, size);

    return ptr;
}