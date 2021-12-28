/**
 * \author Recep Ömer Zorlu
 */

#include "ma_malloc.h"
#include <stdio.h>
#include <string.h>

typedef enum
{
    ALLOCATED,
    FREE
} mem_status;

typedef struct
{
    size size;
    mem_status status;
} mem_chunk_header;

#define MEM_POOL_SIZE 600 // in bytes
#define POINTER_ARRAY_SIZE 20
#define HEADER_SIZE sizeof(mem_chunk_header)
typedef unsigned char byte;

static byte mem_pool[MEM_POOL_SIZE];
static mem_chunk_header *header_pointers[POINTER_ARRAY_SIZE];

// Private Functions
mem_chunk_header *create_new_header(int);
mem_chunk_header *create_new_footer(int, size);

/**
 * Allocates array of bytes (memory pool) and initializes the memory allocator.
 * If some bytes have been used after calling ma_malloc(size), calling to ma_init() will result in clearing up the memory pool.
 */
void ma_init()
{
    printf("\nma_init\n");

    for (unsigned char i = 0; i < POINTER_ARRAY_SIZE; i++)
    {
        header_pointers[i] = NULL;
    }

    int memory_offset = 0;
    header_pointers[0] = create_new_header(memory_offset);
    memory_offset += HEADER_SIZE;
    memory_offset += header_pointers[0]->size;
    header_pointers[1] = create_new_footer(memory_offset, header_pointers[0]->size);
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

    while (counter < POINTER_ARRAY_SIZE - 4)
    {
        mem_chunk_header *cur_h_pointer = header_pointers[counter];

        if (cur_h_pointer != NULL)
        {
            if (cur_h_pointer->status == FREE)
            {
                memory_offset += HEADER_SIZE;
                if (cur_h_pointer->size >= tsize)
                {
                    memory_offset += tsize;

                    cur_h_pointer->size = tsize;
                    cur_h_pointer->status = ALLOCATED;

                    header_pointers[counter + 1]->size = tsize;
                    header_pointers[counter + 1]->status = ALLOCATED;
                    memcpy(mem_pool + memory_offset, header_pointers[counter + 1], HEADER_SIZE);
                    header_pointers[counter + 1] = (mem_chunk_header *)(mem_pool + memory_offset);

                    memory_offset += HEADER_SIZE;

                    header_pointers[counter + 2] = create_new_header(memory_offset);
                    memory_offset += HEADER_SIZE;
                    memory_offset += header_pointers[counter + 2]->size;
                    header_pointers[counter + 3] = create_new_footer(memory_offset, header_pointers[counter + 2]->size);                    
                    
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
                if (counter % 2 == 0)
                {
                    memory_offset += cur_h_pointer->size + (2 * HEADER_SIZE);
                }
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
    printf("\nma_free\n");
    unsigned char i = 0;

    for (i = 0; i < POINTER_ARRAY_SIZE; i++)
    {
        if (header_pointers[i] == (mem_chunk_header *)ptr)
        {
            header_pointers[i]->status = FREE;
            header_pointers[i + 1]->status = FREE;
            break;
        }
    }

    // Check next header
    if ((i + 2) < POINTER_ARRAY_SIZE)
    {
        if (header_pointers[i + 2]->status == FREE)
        {
            header_pointers[i]->size += header_pointers[i + 2]->size + (2 * HEADER_SIZE);
            header_pointers[i + 1]->size += header_pointers[i + 2]->size + (2 * HEADER_SIZE);

            for (unsigned char j = i + 2; j < POINTER_ARRAY_SIZE - 2; j++)
            {
                header_pointers[j] = header_pointers[j + 2];
            }
        }
    }

    // Check previous header
    if ((i - 2) >= 0)
    {
        if (header_pointers[i - 1]->status == FREE)
        {
            header_pointers[i - 2]->size += header_pointers[i]->size + (2 * HEADER_SIZE);
            header_pointers[i - 1]->size += header_pointers[i]->size + (2 * HEADER_SIZE);

            for (unsigned char j = i; j < POINTER_ARRAY_SIZE - 2; j++)
            {
                header_pointers[j] = header_pointers[j + 2];
            }
        }
    }

    printf("Memory freed");
}

/**
 * This function is only for debugging. It prints out the entire memory pool.
 * Print info on headers and content of all elements in your pool
 */
void ma_print(void) {

    //TODO: add your code here
    printf("Print called");
}


mem_chunk_header *create_new_header(int offset)
{
    mem_chunk_header *hp = NULL;

    mem_chunk_header h;
    h.size = (MEM_POOL_SIZE - offset - (2 * HEADER_SIZE));
    h.status = FREE;
    hp = &h;

    memcpy(mem_pool + offset, hp, HEADER_SIZE);

    // overriding the pointer from the "permanent memory"
    hp = (mem_chunk_header *)(mem_pool + offset);

    return hp;
}

mem_chunk_header *create_new_footer(int offset, size size)
{
    mem_chunk_header *hp = NULL;

    mem_chunk_header h;
    h.size = size;
    h.status = FREE;
    hp = &h;

    memcpy(mem_pool + offset, hp, HEADER_SIZE);

    // overriding the pointer from the "permanent memory"
    hp = (mem_chunk_header *)(mem_pool + offset);

    return hp;
}