#include "dplist.h"
#include <stdio.h>
#include <stdlib.h>

void test_insert()
{
    printf("\nInside test insert\n");
    dplist_t *list = dpl_create();
    element_t element = malloc(sizeof(char));
    *element = 'A';
    list = dpl_insert_at_index(list, element, 0);

    element = malloc(sizeof(char));
    *element = 'B';
    list = dpl_insert_at_index(list, element, 1);

    dpl_free(&list);

    printf("\ntest insert finished\n");
}

void test_element_operations()
{
    dplist_t *list = dpl_create();
    element_t element = malloc(sizeof(char));
    *element = 'A';
    list = dpl_insert_at_index(list, element, 0);

    element = malloc(sizeof(char));
    *element = 'B';
    list = dpl_insert_at_index(list, element, 1);

    element = malloc(sizeof(char));
    *element = 'C';
    list = dpl_insert_at_index(list, element, 1);

    element_t cur_element = dpl_get_element_at_index(list, 1);
    printf("current element is => %c\n", *cur_element);

    int index = dpl_get_index_of_element(list, cur_element);
    printf("current element is at index => %d\n", index);

    dpl_free(&list);

    printf("\ntest element operations finished\n");
}

void test_remove()
{
    dplist_t *list = dpl_create();
    element_t element = malloc(sizeof(char));
    *element = 'A';
    list = dpl_insert_at_index(list, element, 0);

    element = malloc(sizeof(char));
    *element = 'B';
    list = dpl_insert_at_index(list, element, 1);

    element = malloc(sizeof(char));
    *element = 'C';
    list = dpl_insert_at_index(list, element, 1);

    // beginning
    list = dpl_remove_at_index(list, 0);
    element = malloc(sizeof(char));
    *element = 'A';
    list = dpl_insert_at_index(list, element, 0);

    // middle
    element = malloc(sizeof(char));
    *element = 'B';
    list = dpl_remove_at_index(list, 1);
    list = dpl_insert_at_index(list, element, 1);

    // end
    element = malloc(sizeof(char));
    *element = 'C';
    list = dpl_remove_at_index(list, 3);
    list = dpl_insert_at_index(list, element, 2);

    dpl_free(&list);

    printf("\ntest remove finished\n");
}

int main(int argc, char const *argv[])
{
    printf("\nInside main\n");
    test_insert();

    test_remove();

    test_element_operations();
    return 0;
}
