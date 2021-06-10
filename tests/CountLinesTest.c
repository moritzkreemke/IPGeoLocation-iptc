//
// Created by moritz on 04/06/2021.
//
#include "Tests.h"


int main ()
{

    assert(countlines("data/asn-country-ipv4.csv") == 5);
    assert(countlines("blabla") == 0);

    return 0;
}

