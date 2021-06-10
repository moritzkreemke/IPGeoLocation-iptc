//
// Created by moritz on 09/06/2021.
//
#include "Tests.h"


int main ()
{
    assert(IsIPV4("127.0.0.1") == 1);
    assert(IsIPV4("127.0.0.1\n") == 0);
    assert(IsIPV4("blabla127.0.0.1bla") == 0);
    assert(IsIPV4("rhost=222.168.42.7") == 0);
    assert(IsIPV4("77.11.10.200") == 1);


    return 0;
}