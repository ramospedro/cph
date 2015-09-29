#include <iostream>
#include "include/CPH.h"

using namespace std;


int main()
{
    //string netPath = "/home/pedro/Dropbox/2015-DeteccaoComunidadesSobreposicao (1)/desenvolvimento/instances/normalizadas/email.net";
    string netPath = "/home/pedro/Dropbox/2015-DeteccaoComunidadesSobreposicao (1)/desenvolvimento/instances/pedro/teste2.net";
    detectCommunities(netPath, 3, 6);

    return 0;
}
