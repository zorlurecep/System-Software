#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
    This char definition is not required at this point.
    Instead of "char" I can basically use "void".
    I can't use "void" with pointer functions.
    Throws error non-void function should have return type.

    char *scan_variable(char *PrintText, char *variable)
    {
        printf("%s", PrintText);
        scanf("%s", variable);
        return variable;
}
*/

void scan_variable(char *PrintText, char *variable)
{
    // To limit print length, we can use the length with placeholder.
    printf("%.200s", PrintText);
    // To limit the scan length, we can use the length with placeholder.
    scanf("%100s", variable);
}

void conver_to_uppercase(char *StringToChange, char *StringToReplace)
{
    for (int i = 0; i < strlen(StringToChange); i++)
    {
        StringToReplace[i] = toupper(StringToChange[i]);
    }
    printf("%s\n%s\n", StringToChange, StringToReplace);
}

void compare_strings(char *str1, char *str2)
{
    int result;
    /*
        0   --> string are equal
        >0  --> if first string has bigger value in ASCII
        0<  --> if the second string has bigger value in ASCII
    */
    result = strcmp(str1, str2); // uppercase values has smaller ASCII numbers
    printf("%d\n", result);

    result = strcasecmp(str1, str2); // case insensitive comparision
    printf("%d\n", result);
}


// what is the difference between scanf and fscanf
// I feel like they are two separate things. scanf from terminal and fscanf is from files.
int main()
{
    char first[100], second[100], str[100]; // name[100], str[100];

    scan_variable("First Name: ", first);
    scan_variable("Second Name: ", second);

    conver_to_uppercase(second, str);

    compare_strings(second, str);

    return 0;
}
