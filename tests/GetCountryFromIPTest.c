//
// Created by moritz on 06/06/2021.
//
#include "Tests.h"

int main ()
{
    struct Entry *arr;
    int lines = countlines(FILE_PATH);
    arr = calloc(lines, sizeof (struct Entry));
    parseIPFile(arr, FILE_PATH);

    char result[2];

    getCountryFromIP(result, "1.0.0.0", arr, lines);
    assert(("1.0.0.0", strcmp(result, "AU") == 0));

    getCountryFromIP(result, "1.0.0.1", arr, lines);
    assert(("1.0.0.1", strcmp(result, "AU") == 0));

    getCountryFromIP(result, "1.0.0.255", arr, lines);
    assert(("1.0.0.255", strcmp(result, "AU") == 0));

    free(arr);

    return 0;
}
