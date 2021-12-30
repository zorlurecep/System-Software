/**
 * \author Recep Ömer Zorlu
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

//#define DEBUG

/*
 * definition of error codes
 */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1          // error due to mem alloc failure
#define DPLIST_INVALID_ERROR 2         // error due to a list operation applied on a NULL list
#define DPLIST_INVALID_INDEX 3         // error due to trying to access indexes smaller than 0
#define DPLIST_INDEX_OUT_OF_BOUNDARY 4 // error due to trying to access indexes bigger than length of the list
#define DPLIST_NODE_ERROR 5            // error due to node doesn't exist

#ifdef DEBUG
#define DEBUG_PRINTF(...)                                                                    \
    do                                                                                       \
    {                                                                                        \
        fprintf(stderr, "\nIn %s - function %s at line %d: ", __FILE__, __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                                                        \
        fflush(stderr);                                                                      \
    } while (0)
#else
#define DEBUG_PRINTF(...) (void)0
#endif

#define DPLIST_ERR_HANDLER(condition, err_code)                                 \
    do                                                                          \
    {                                                                           \
        if ((condition))                                                        \
            DEBUG_PRINTF(#condition " failed with error code " #err_code "\n"); \
        assert(!(condition));                                                   \
    } while (0)

/*
 * The real definition of struct list / struct node
 */
struct dplist_node
{
    dplist_node_t *prev, *next;
    element_t element;
};

struct dplist
{
    dplist_node_t *head;
    // more fields will be added later
};

dplist_t *dpl_create()
{
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_MEMORY_ERROR);
    list->head = NULL;
    // pointer drawing breakpoint
    return list;
}

void dpl_free(dplist_t **list)
{
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    dplist_t *cur_list = *list;
    DPLIST_ERR_HANDLER(cur_list == NULL, DPLIST_INVALID_ERROR);

    if (cur_list->head == NULL)
    {
        return;
    }
    int size = dpl_size(cur_list);
    for (int counter = 0; counter < size; counter++)
    {
        cur_list = dpl_remove_at_index(cur_list, 0);
    }

    free(*list);
    *list = NULL;
}

/* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default pointer manipulation
 * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
 **/

dplist_t *dpl_insert_at_index(dplist_t *list, element_t element, int index)
{
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL)
        return NULL;

    list_node = malloc(sizeof(dplist_node_t));
    DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_MEMORY_ERROR);
    list_node->element = element;
    // pointer drawing breakpoint
    if (list->head == NULL && index >= 0)
    { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    }
    else if (index == 0)
    { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    }
    else if (index < 0)
    {
        free(list_node);
        list_node = NULL;
        DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_INVALID_INDEX);
    }
    else
    {
        ref_at_index = dpl_get_reference_at_index(list, index);
        int length = dpl_size(list);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < length)
        { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        }
        // If the given index is out of boundary of the list
        else if (index > length)
        {
            free(list_node);
            list_node = NULL;
            DPLIST_ERR_HANDLER(index > length, DPLIST_INDEX_OUT_OF_BOUNDARY);
        }
        else
        { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }
    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index)
{
    DPLIST_ERR_HANDLER(index < 0, DPLIST_INVALID_INDEX);
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    dplist_node_t *cur_node = dpl_get_reference_at_index(list, index);
    // at beginning    
    if (cur_node->prev == NULL)
    {
        int size = dpl_size(list);
        // length > 1
        if (size > 1)
        {
            cur_node->next->prev = NULL;
            list->head = cur_node->next;            
            free(cur_node);
            cur_node = NULL;
        }
        // length = 1
        else
        {
            list->head = NULL;
            free(cur_node);
            cur_node = NULL;
        }
    }
    // at middle
    else if (cur_node->prev != NULL && cur_node->next != NULL)
    {
        cur_node->next->prev = cur_node->prev;
        cur_node->prev->next = cur_node->next;        
        free(cur_node);
        cur_node = NULL;
    }
    // at end
    else
    {
        cur_node->prev->next = NULL;        
        free(cur_node);
        cur_node = NULL;
    }

    return list;
}

int dpl_size(dplist_t *list)
{
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    if (list->head == NULL)
    {
        return 0;
    }
    else
    {
        int count;
        dplist_node_t *cur_node;
        for (count = 1, cur_node = list->head; cur_node->next != NULL; cur_node = cur_node->next, count++)
        {
        }
        return count;
    }
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index)
{
    int count;
    dplist_node_t *dummy;
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    if (list->head == NULL)
        return NULL;
    for (dummy = list->head, count = 0; dummy->next != NULL; dummy = dummy->next, count++)
    {
        if (count >= index)
            return dummy;
    }
    return dummy;
}

element_t dpl_get_element_at_index(dplist_t *list, int index)
{
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    dplist_node_t *element_node = dpl_get_reference_at_index(list, index);
    DPLIST_ERR_HANDLER(element_node == NULL, DPLIST_NODE_ERROR);
    return element_node->element;
}

int dpl_get_index_of_element(dplist_t *list, element_t element)
{
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);

    int count;
    dplist_node_t *cur_node;
    for (count = 0, cur_node = list->head; cur_node->next != NULL; cur_node = cur_node->next, count++)
    {
        if (cur_node->element == element)
        {
            return count;
        }
    }
    // Element doesn't exists
    return -1;
}
