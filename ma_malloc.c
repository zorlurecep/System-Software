/**
 * \author Recep Ömer Zorlu
 */

#include "ma_malloc.h"
#include <stdio.h>
#include <string.h>

#define MEM_POOL_SIZE 600 //in bytes
typedef unsigned char byte;

//! An enum of statuses that can be used in the header
typedef enum
{
    ALLOCATED,
    FREE
} mem_status;

//! Every item in the pool will have at least a header with size and status of the following data
typedef struct
{
    size size;
    mem_status status;
} mem_chunk_header;

static byte mem_pool[MEM_POOL_SIZE];

/**
 * Allocates array of bytes (memory pool) and initializes the memory allocator.
 * If some bytes have been used after calling ma_malloc(size), calling to ma_init() will result in clearing up the memory pool.
 */
void ma_init()
{
    printf("\nma_init\n");    

    mem_chunk_header *header_pointer = NULL;
    

    mem_chunk_header header;

    header.size = (MEM_POOL_SIZE - sizeof(mem_chunk_header));
    
    header.status = FREE;
    
    header_pointer = &header;
    
    memcpy(mem_pool, header_pointer, sizeof(mem_chunk_header));
    
    mem_chunk_header h2;
    
    memcpy(&h2, mem_pool, sizeof(mem_chunk_header));    
    printf("\nmemory allocation not failed\n");
    printf("header %d and %d", h2.size, h2.status);
    ma_print();
}

/**
 * Requesting for the tsize bytes from memory pool.
 * If the request is possible, the pointer to the first possible address byte (right after its header) in memory pool is returned.
 */
// void *ma_malloc(size tsize)
// {

//     //TODO: add your code here
//     printf("ma_malloc");
// }

/**
 * Releasing the bytes in memory pool which was hold by ptr, meaning makes those bytes available for other uses.
 * Implement also the coalescing behavior.
 */
void ma_free(void *ptr)
{

    //TODO: add your code here
    printf("ma_free");
}

/**
 * This function is only for debugging. It prints out the entire memory pool.
 * Print info on headers and content of all elements in your pool
 */
void ma_print(void)
{

    //TODO: add your code here
    // for (int i = 0; i < MEM_POOL_SIZE; i++)
    // {
    //     printf("%d\n", mem_pool[i]);
    // }

    for (int i = 0; i < 15; i++)
    {
        printf("%d\n", mem_pool[i]);
    }
}
