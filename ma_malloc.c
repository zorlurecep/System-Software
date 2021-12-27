/**
 * \author Recep Ömer Zorlu
 */

#include "ma_malloc.h"
#include <stdio.h>
#include <string.h>

#define MEM_POOL_SIZE 600 // in bytes
typedef unsigned char byte;

static byte mem_pool[MEM_POOL_SIZE];
static mem_chunk_header *header_pointers[7]; //= {NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // size is 7

// Private Functions
mem_chunk_header *create_new_header(int);

/**
 * Allocates array of bytes (memory pool) and initializes the memory allocator.
 * If some bytes have been used after calling ma_malloc(size), calling to ma_init() will result in clearing up the memory pool.
 */
void ma_init()
{
    printf("\nma_init\n");

    for (unsigned char i = 0; i < 7; i++)
    {
        header_pointers[i] = NULL;
    }

    mem_chunk_header *hp = NULL;
    mem_chunk_header h;
    h.size = (MEM_POOL_SIZE - sizeof(mem_chunk_header));
    h.status = FREE;
    hp = &h;

    memcpy(mem_pool, hp, sizeof(mem_chunk_header));

    header_pointers[0] = (mem_chunk_header *)mem_pool;
    printf("asdasd");
}

/**
 * Requesting for the tsize bytes from memory pool.
 * If the request is possible, the pointer to the first possible address byte (right after its header) in memory pool is returned.
 */
void *ma_malloc(size tsize)
{
    printf("\nma_malloc\n");
    unsigned char counter = 0;
    int memory_offset = 0;

    while (counter < 7)
    {
        mem_chunk_header *cur_h_pointer = header_pointers[counter];
        // mem_chunk_header cur_h;
        // memcpy(&cur_h, mem_pool, sizeof(mem_chunk_header));

        if (cur_h_pointer != NULL)
        {
            if (cur_h_pointer->status == FREE)
            {
                memory_offset += 8;
                if (cur_h_pointer->size >= tsize)
                {
                    memory_offset += tsize;

                    cur_h_pointer->size = tsize;
                    cur_h_pointer->status = ALLOCATED;

                    memcpy(mem_pool + (memory_offset - (tsize + 8)), cur_h_pointer, sizeof(mem_chunk_header));
                    printf("Memory allocated");

                    // create new header
                    if (counter + 1 < 7)
                    {
                        printf("New chunk created");
                        header_pointers[counter + 1] = create_new_header(memory_offset);
                    }
                    // add the header to header array
                    return cur_h_pointer;
                }
                else
                {
                    printf("Not enough space in the memory");
                    return NULL;
                }
            }
            else
            {
                memory_offset += cur_h_pointer->size + sizeof(mem_chunk_header);
            }
        }
        else
        {
            printf("Not enough space in the memory 2\n");
            return NULL;
        }

        counter++;
    }

    return NULL;
}

/**
 * Releasing the bytes in memory pool which was hold by ptr, meaning makes those bytes available for other uses.
 * Implement also the coalescing behavior.
 */
void ma_free(void *ptr)
{
    printf("ma_free");
}

/**
 * This function is only for debugging. It prints out the entire memory pool.
 * Print info on headers and content of all elements in your pool
 */
void ma_print(mem_chunk_header *header_pointer)
{
    printf("Memory header with size = %d and status = %d\n", header_pointer->size, header_pointer->status);
}

mem_chunk_header *create_new_header(int offset)
{
    mem_chunk_header *hp = NULL;

    mem_chunk_header h;
    h.size = (MEM_POOL_SIZE - offset - sizeof(mem_chunk_header));
    h.status = FREE;
    hp = &h;

    memcpy(mem_pool + offset, hp, sizeof(mem_chunk_header));

    // overriding the pointer from the "permanent memory"
    hp = (mem_chunk_header *)(mem_pool + offset);

    return hp;
}