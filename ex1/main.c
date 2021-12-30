#include "dplist.h"
#include <stdio.h>

void test_insert()
{
    dplist_t *list = dpl_create();
    list = dpl_insert_at_index(list, 'A', 0);
    list = dpl_insert_at_index(list, 'B', 1);
    list = dpl_insert_at_index(list, 'C', 2);
    list = dpl_insert_at_index(list, 'D', 3);
    list = dpl_insert_at_index(list, 'E', 4);
    list = dpl_insert_at_index(list, 'F', 5);
    list = dpl_insert_at_index(list, 'G', 6);

    // Insert to beginning
    list = dpl_insert_at_index(list, 'X', 0);
    // Insert to middle
    list = dpl_insert_at_index(list, 'Y', 3);
    // Insert to end
    list = dpl_insert_at_index(list, 'Z', dpl_size(list));

    dpl_free(&list);

    printf("\ntest insert finished\n");
}

void test_remove(){
    dplist_t *list = dpl_create();
    list = dpl_insert_at_index(list, 'A', 0);
    list = dpl_insert_at_index(list, 'B', 1);
    list = dpl_insert_at_index(list, 'C', 2);
    list = dpl_insert_at_index(list, 'D', 3);

    //beginning
    list = dpl_remove_at_index(list, 0);
    list = dpl_insert_at_index(list, 'A', 0);
    // middle
    list = dpl_remove_at_index(list, 1);
    list = dpl_insert_at_index(list, 'B', 1);
    // end
    list = dpl_remove_at_index(list, 3);
    list = dpl_insert_at_index(list, 'D', 3);

    dpl_free(&list);    

    printf("\ntest remove finished\n");
}

void test_element_operations(){
    dplist_t *list = dpl_create();
    list = dpl_insert_at_index(list, 'A', 0);
    list = dpl_insert_at_index(list, 'B', 1);
    list = dpl_insert_at_index(list, 'C', 2);
    list = dpl_insert_at_index(list, 'D', 3);
    list = dpl_insert_at_index(list, 'E', 4);
    list = dpl_insert_at_index(list, 'F', 5);
    list = dpl_insert_at_index(list, 'G', 6);

    element_t cur_element = dpl_get_element_at_index(list, 4);
    printf("current element is => %c\n", cur_element);

    int index = dpl_get_index_of_element(list, cur_element);
    printf("current element is at index => %d\n", index);

    dpl_free(&list);    

    printf("\ntest element operations finished\n");
}

int main(int argc, char const *argv[])
{
    test_insert();

    test_remove();

    test_element_operations();
    return 0;
}
