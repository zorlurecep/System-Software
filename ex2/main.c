#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./lib/dplist.h"

typedef struct
{
    int id;
    char *name;
} my_element_t;

void *element_copy(void *element);
void element_free(void **element);
int element_compare(void *x, void *y);

void *element_copy(void *element)
{
    my_element_t *copy = malloc(sizeof(my_element_t));
    char *new_name;
    // asprintf(&new_name, "%s", ((my_element_t *)element)->name);
    new_name = malloc(sizeof(char));
    memcpy(new_name, ((my_element_t *)element)->name, sizeof(char));
    assert(copy != NULL);
    copy->id = ((my_element_t *)element)->id;
    copy->name = new_name;
    return (void *)copy;
}

void element_free(void **element)
{
    free((((my_element_t *)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y)
{
    return ((((my_element_t *)x)->id < ((my_element_t *)y)->id) ? -1 : (((my_element_t *)x)->id == ((my_element_t *)y)->id) ? 0
                                                                                                                            : 1);
}

void test_insert_with_copy()
{
    printf("\nInside test insert\n");
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    my_element_t *element = malloc(sizeof(my_element_t));
    element->id = 0;
    char *element_char = malloc(sizeof(char));
    *element_char = 'A';
    element->name = element_char;
    list = dpl_insert_at_index(list, element, 0, true);

    element = malloc(sizeof(my_element_t));
    element->id = 1;
    element_char = malloc(sizeof(char));
    *element_char = 'B';
    element->name = element_char;
    list = dpl_insert_at_index(list, element, 1, true);

    dpl_free(&list, true);

    printf("\ntest insert finished\n");
}

int main(int argc, char const *argv[])
{
    test_insert_with_copy();
    return 0;
}
