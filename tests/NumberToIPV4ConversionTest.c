//
// Created by moritz on 09/06/2021.
//
#include "Tests.h"


int main ()
{
    char* result[15];
    convertNumberToIPv4(result, 16777216);
    assert(strcmp(result, "1.0.0.0") == 0);

    convertNumberToIPv4(result, 1312253709);
    assert(strcmp(result, "78.55.103.13") == 0);

    convertNumberToIPv4(result, 1292569288);
    assert(strcmp(result, "77.11.10.200") == 0);


    return 0;
}
