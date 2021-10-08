#include<stdio.h>

int main(){
    char *pointer; //regardless of the type of it it has the same size

    printf("int: ");
    printf("%lu\n", sizeof(int)); // %lu -> long unsigned integer    
    printf("float: ");
    printf("%lu\n", sizeof(float));    
    printf("double: ");
    printf("%lu\n", sizeof(double));
    printf("void: ");
    printf("%lu\n", sizeof(void));
    printf("pointer: ");
    printf("%lu\n", sizeof(pointer));
    return 0;
}