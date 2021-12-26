#include <stdio.h>
#include <stdlib.h>

/**
 * Drawing function showed that d variable in the main is pointing to nothing
 * when the date_struct method is called.
 * We should either return a pointer to d vairable or use
 * *date = dummy
 */
// typedef struct
// {
//     short day, month;
//     unsigned year;
// } date_t;
// void date_struct(int day, int month, int year, date_t *date)
// {
//     date_t dummy;
//     dummy.day = (short)day;
//     dummy.month = (short)month;
//     dummy.year = (unsigned)year;
//     //*date = dummy; correct version of this solution will be this
//     date = &dummy;
// }
// int main(void)
// {
//     int day, month, year;
//     date_t d;
//     printf("\nGive day, month, year:");
//     scanf("%d %d %d", &day, &month, &year);
//     date_struct(day, month, year, &d);
//     printf("\ndate struct values: %d-%d-%d \n", d.day, d.month, d.year);
//     return 0;
// }

typedef struct
{
    short day, month;
    unsigned year;
} date_t;

void f(void)
{
    //int x, y, z;
    //printf("%d %d %d\n", x, y, z);
}

date_t *date_struct(int day, int month, int year)
{
    // Below implementation is wrong
    // date_t dummy;
    // dummy.day = (short)day;
    // dummy.month = (short)month;
    // dummy.year = (unsigned)year;
    // return &dummy;

    date_t *dummy = malloc(sizeof(date_t));
    (*dummy).day = (short)day;
    dummy->month = (short)month;
    dummy->year = (unsigned)year;
    return dummy;
}
int main(void)
{
    int day, month, year;
    date_t *d;
    printf("\nGive day, month, year:");
    scanf("%d %d %d", &day, &month, &year);
    d = date_struct(day, month, year);
    //f();
    printf("\ndate struct values: %d-%d-%d", d->day, d->month, d->year);
    return 0;
}