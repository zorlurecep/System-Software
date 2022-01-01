/**
 * \author Recep Ömer Zorlu
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

/*
 * definition of error codes
 * */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1  // error due to mem alloc failure
#define DPLIST_INVALID_ERROR 2 // error due to a list operation applied on a NULL list

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

#define DPLIST_ERR_HANDLER(condition, err_code)   \
    do                                            \
    {                                             \
        if ((condition))                          \
            DEBUG_PRINTF(#condition " failed\n"); \
        assert(!(condition));                     \
    } while (0)

/*
 * The real definition of struct list / struct node
 */

struct dplist_node
{
    dplist_node_t *prev, *next;
    void *element;
};

struct dplist
{
    dplist_node_t *head;

    void *(*element_copy)(void *src_element);

    void (*element_free)(void **element);

    int (*element_compare)(void *x, void *y);
};

dplist_t *dpl_create( // callback functions
    void *(*element_copy)(void *src_element),
    void (*element_free)(void **element),
    int (*element_compare)(void *x, void *y))
{
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_MEMORY_ERROR);
    list->head = NULL;
    list->element_copy = element_copy;
    list->element_free = element_free;
    list->element_compare = element_compare;
    return list;
}

void dpl_free(dplist_t **list, bool free_element)
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
        cur_list = dpl_remove_at_index(cur_list, 0, free_element);
    }

    free(*list);
    *list = NULL;
}

dplist_t *dpl_insert_at_index(dplist_t *list, void *element, int index, bool insert_copy)
{
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL)
    {
        return NULL;
    }

    list_node = malloc(sizeof(dplist_node_t));
    DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_MEMORY_ERROR);
    if (insert_copy == true)
    {
        list_node->element = list->element_copy(element);
        list->element_free(&element);
    }
    else
    {
        list_node->element = element;
    }

    if (list->head == NULL)
    {
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    }
    else if (index <= 0)
    {
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    }
    else
    {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list))
        {
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        }
        else
        {
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint}
        }
    }

    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index, bool free_element)
{
    if (list == NULL)
        return NULL;

    if (list->head == NULL)
        return list;

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
        }
        // length = 1
        else
        {
            list->head = NULL;
        }
    }
    // at middle
    else if (cur_node->prev != NULL && cur_node->next != NULL)
    {
        cur_node->next->prev = cur_node->prev;
        cur_node->prev->next = cur_node->next;
    }
    // at end
    else
    {
        cur_node->prev->next = NULL;
    }

    if (free_element == true)
    {
        list->element_free(&(cur_node->element));
        free(cur_node);
        cur_node = NULL;
    }
    else
    {
        free(cur_node);
        cur_node = NULL;
    }

    return list;
}

int dpl_size(dplist_t *list)
{
    if (list == NULL || list->head == NULL)
    {
        return -1;
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

void *dpl_get_element_at_index(dplist_t *list, int index)
{
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    dplist_node_t *element_node = dpl_get_reference_at_index(list, index);
    return element_node->element;
}

int dpl_get_index_of_element(dplist_t *list, void *element)
{
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);

    int count;
    dplist_node_t *cur_node;
    for (count = 0, cur_node = list->head; cur_node->next != NULL; cur_node = cur_node->next, count++)
    {
        if ((list->element_compare(cur_node->element, element)) == 0)
        {
            return count;
        }
    }
    // Because last element's next always points to the null
    // last element doesn't get compared
    // We should check for it outside of the loop
    if ((list->element_compare(cur_node->element, element)) == 0)
    {
        return count;
    }
    // Element doesn't exists
    return -1;
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index)
{
    int count;
    dplist_node_t *dummy;

    if (list == NULL || list->head == NULL)
        return NULL;

    for (dummy = list->head, count = 0; dummy->next != NULL; dummy = dummy->next, count++)
    {
        if (count >= index)
            return dummy;
    }
    return dummy;
}

// Reference can have element in it but that reference should be in the list
void *dpl_get_element_at_reference(dplist_t *list, dplist_node_t *reference)
{
    if (list == NULL || list->head == NULL || reference == NULL)
        return NULL;

    dplist_node_t *dummy;
    for (dummy = list->head; dummy->next != NULL; dummy = dummy->next)
    {
        if (dummy == reference)
            return dummy->element;
    }
    // Because last element's next always points to the null
    // last element doesn't get compared
    // We should check for it outside of the loop
    if (dummy == reference)
        return dummy->element;
    return NULL;
}
