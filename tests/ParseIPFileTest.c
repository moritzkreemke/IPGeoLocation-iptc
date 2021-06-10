//
// Created by moritz on 04/06/2021.
//
#include "Tests.h"



int main ()
{
    int lineCount = countlines(FILE_PATH);
    struct Entry *arr;
    arr = calloc(lineCount, sizeof (struct Entry));
    parseIPFile(arr, FILE_PATH);

    assert(strcmp(arr[0].Country, "AU") == 0);
    assert(arr[0].startRange == 16777216);
    assert(arr[0].endRange == 16777471);

    assert(strcmp(arr[3].Country, "RU") == 0);

    assert(strcmp(arr[4].Country, "US") == 0);
    assert(arr[4].startRange == 3234553856);
    assert(arr[4].endRange == 3234554623);


    free(arr);

    return 0;
}

