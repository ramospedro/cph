#ifndef CLIQUESEARCHER_H
#define CLIQUESEARCHER_H

#include "cliquealgorithm.h"

//CONTEXT
class CliqueSearcher
{

    private:

        CliqueAlgorithm *_cliqueSA;

    public:

        CliqueSearcher(CliqueAlgorithm *_cliqueSA);

        void findCliques();

        int quantityOfCliques();

        int quantityOfCalls();

        string getHeuristicUsed();

        float getAlphaUsed();

        list<set<unsigned>> getCliqueList();

        void printCliques();


};

#endif // CLIQUESEARCHER_H

