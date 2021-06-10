//
// Created by moritz on 08/06/2021.
//
#include "Tests.h"
#include "ncurses.h"
struct Cars
{
    int value1;
    int value2;
};

struct Cars* create5Cars ()
{
    struct Cars* arr = NULL;

    arr = realloc(arr, 5 * sizeof (struct Cars));
    for(int i = 0; i < 5; i++)
    {
        arr[i].value1 = i;
        arr[i].value2 = i + 5;
    }

    return arr;
}

void modifyNumber (int* num)
{
    int lol = 7;
    *num = lol;

}

int main ()
{
    /*int lol;
    modifyNumber(&lol);
    printf("%d", lol);

    struct Cars* arr = create5Cars();

    for(int i = 0; i < 5; i++)
    {
        printf("v1 %d v2 %d\n", arr[i].value1, arr[i].value2);
    }

    free(arr);

    struct Cars* arr2;
    free(arr2);*/

    for(int i = 0; i < 5; i++)
    {
        printf("\rLine 1 %d", i);
        fflush(stdout);
        printf("\n\rLine 2 %d", i + 1);
        fflush(stdout);
        /*printf("\rLine 3 %d", i + 2);
        printf("\rLine 4 %d", i + 3);
        printf("\rLine 5 %d", i + 4);*/

    }

    return 0;
}
