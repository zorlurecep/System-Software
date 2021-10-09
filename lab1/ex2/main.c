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

    printf("Part2 ------------------------------------------\n");
    printf("short int: ");
    printf("%lu\n", sizeof(short int));
    printf("int: ");
    printf("%lu\n", sizeof(int));
    printf("long int: ");
    printf("%lu\n", sizeof(long int));

    printf("Part3 ------------------------------------------\n");
    int result = ((char)(-1));
    printf("%d\n",result);
    printf("if negative signed char used in this system if positive unsigned is used\n");

    return 0;
}