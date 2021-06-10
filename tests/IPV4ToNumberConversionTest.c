//
// Created by moritz on 04/06/2021.
//
#include "Tests.h"


int main ()
{
    assert(convertIPv4ToNumber("1.0.0.0") == 16777216);
    assert(convertIPv4ToNumber("78.55.103.13") == 1312253709);
    assert(convertIPv4ToNumber("77.11.10.200\n") == 1292569288);

    return 0;
}