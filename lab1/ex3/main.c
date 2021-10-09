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

/*
    **********SAFE***********
    strncpy --> copy up to certain length
    strncat --> concatenate up to certain length
    **********UNSAFE***********
    strcpy --> copy without length limitation
    strcat --> concatenate without length limitation
*/
void concatenate_strings(char *str1, char *str2, char *ConcatenatedString)
{
    strncpy(ConcatenatedString, str1, 100);
    strncat(ConcatenatedString, str2, 100);
    printf("%s\n", ConcatenatedString);
}

int read_birthday()
{
    int year;
    printf("Birth Year: ");
    scanf("%d", &year);
    return year;
}

void concatenate_strings_others(char *str1, char *str2, int year, char *ConcatenatedString)
{
    /**
     * sprintf --> concatenates strings and other types and stores on char buffer without size limitation
     * snprintf --> concatenates strings and other types and stores on char buffer up to n characters
    */

    snprintf(ConcatenatedString, 200, "\n%s %s %d\n", str1, str2, year);
    printf("%s", ConcatenatedString);
}

void scan_multiple_variable(char *first, char *second, int year)
{
    // Scan from a source inside the program rather than terminal line
    char source[200];
    strncpy(source, "Hello World 2", 200);
    sscanf(source, "%s %s %d", first, second, &year);
    printf("%s %s %d\n", first, second, year);
}

// what is the difference between scanf and fscanf
// I feel like they are two separate things. scanf from terminal and fscanf is from files.
int main()
{
    char first[100], second[100], str[100], name[201];
    int year;

    scan_variable("First Name: ", first);
    scan_variable("Second Name: ", second);

    conver_to_uppercase(second, str);

    compare_strings(second, str);

    concatenate_strings(first, second, name);

    year = read_birthday();

    // To empty name because it is used earlier
    memset(name, 0, 200); // array to empty, what to fill in instead?, length to clear
    concatenate_strings_others(first, second, year, name);

    scan_multiple_variable(first, second, year);

    return 0;
}
