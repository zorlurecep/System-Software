#define _GNU_SOURCE
#include "dplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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

void test_insert_without_copy()
{
    printf("\nInside test insert\n");
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);
    my_element_t *element = malloc(sizeof(my_element_t));
    element->id = 0;
    char *element_char = malloc(sizeof(char));
    *element_char = 'A';
    element->name = element_char;
    list = dpl_insert_at_index(list, element, 0, false);

    element = malloc(sizeof(my_element_t));
    element->id = 1;
    element_char = malloc(sizeof(char));
    *element_char = 'B';
    element->name = element_char;
    list = dpl_insert_at_index(list, element, 1, false);

    dpl_free(&list, false);

    printf("\ntest insert finished\n");
}

void test_element_operations()
{
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

    element = malloc(sizeof(my_element_t));
    element->id = 2;
    element_char = malloc(sizeof(char));
    *element_char = 'C';
    element->name = element_char;
    list = dpl_insert_at_index(list, element, 2, true);

    element = malloc(sizeof(my_element_t));
    element->id = 3;
    element_char = malloc(sizeof(char));
    *element_char = 'D';
    element->name = element_char;
    list = dpl_insert_at_index(list, element, 3, true);

    my_element_t *cur_element = (my_element_t *)dpl_get_element_at_index(list, 0);
    printf("current element id => %d and name => %c\n", cur_element->id, *((char *)cur_element->name));

    int index = dpl_get_index_of_element(list, (void *)cur_element);
    printf("current element is at index => %d\n", index);

    dplist_node_t *node_one = dpl_get_reference_at_index(list, 1);
    printf("Current reference at index is gathered\n");

    cur_element = dpl_get_element_at_reference(list, node_one);
    printf("Current element at reference is, id => %d and name => %c\n", cur_element->id, *((char *)cur_element->name));

    dpl_free(&list, true);

    printf("\ntest element operations finished\n");
}

int main(int argc, char const *argv[])
{
    printf("\nInside main\n");

    test_insert_with_copy();
    // test_insert_without_copy();

    // test_remove();

    test_element_operations();
    return 0;
}
