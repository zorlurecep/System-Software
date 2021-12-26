#include <stdio.h>

void swap_pointers(void **, void **);

int main(int argc, char const *argv[])
{
    int a = 1;
    int b = 2;

    int *p = &a;
    int *q = &b;

    printf("\naddress of p = %p and q = %p\n", p, q);

    swap_pointers((void *)&p, (void *)&q);
    
    printf("\naddress of p = %p and q = %p\n", p, q);
    printf("\naddress of p = %d and q = %d\n", *p, *q);

    return 0;
}

// p1 and p2 is a double pointer
// Below implementation changes values of the variables not the pointers

// void swap_pointers(void *p1, void *p2)
// {
//     int temp = *(int *)p1;
//     *(int *) p1 = *(int *)p2;
//     *(int *) p2 = temp;
// }

// I have pointers that are pointing to a pointer. So it is a double pointer.
// then I access the values inside the first level pointers and swap them.
void swap_pointers(void **p1, void **p2)
{    
    int *temp = *p1;
    *p1 = *p2;
    *p2 = temp;    
}