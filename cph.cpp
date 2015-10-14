#include <iostream>
#include "include/CPH.h"
#include <fstream>

using namespace std;


int main(int argc, char *argv[])
{

    //string netPath = "/home/pedro/Dropbox/2015-DeteccaoComunidadesSobreposicao (1)/desenvolvimento/instances/normalizadas/email.net";
    //string netPath = "/home/pedro/Dropbox/2015-DeteccaoComunidadesSobreposicao (1)/desenvolvimento/instances/pedro/teste3.net";

    string netPath = argv[1];
    string alphaString = argv[2];
    string cliqueAlgorithmString = argv[3];
    string maxIterationsModulesString = argv[4];
    string maxIterationsNodesString = argv[5];
    string resultsDirectory = argv[6];
    string experiment = argv[7];

    float alpha = stod(alphaString);
    unsigned minK = 3;
    unsigned cliqueAlgorithm = stod(cliqueAlgorithmString);
    unsigned maxIterationsModules = stod(maxIterationsModulesString);
    unsigned maxIterationsNodes = stod(maxIterationsNodesString);

    detectCommunities(netPath, minK, alpha, cliqueAlgorithm, maxIterationsModules, maxIterationsNodes, resultsDirectory, experiment);

    return 0;
}
