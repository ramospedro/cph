#include <iostream>

using namespace std;

#include "include/NetConverter.h"

int convert(int argc, char *argv[])
{

    string datPath = argv[1];
    string netDirectory = argv[2];

    datToNet(datPath, netDirectory);

    return 0;
}
