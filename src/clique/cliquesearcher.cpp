#include "cliquesearcher.h"

CliqueSearcher::CliqueSearcher(CliqueAlgorithm *_cliqueSA)
{
    this->_cliqueSA = _cliqueSA;
}


void CliqueSearcher::findCliques()
{
    this->_cliqueSA->execute();
}


int CliqueSearcher::quantityOfCliques()
{
    return _cliqueSA->cliques.size();
}

int CliqueSearcher::quantityOfCalls()
{
    return _cliqueSA->chamadasRecursivas;
}


string CliqueSearcher::getHeuristicUsed()
{
    return _cliqueSA->getName();
}


float CliqueSearcher::getAlphaUsed()
{
    return _cliqueSA->getAlpha();
}

list<set<unsigned>>  CliqueSearcher::getCliqueList()
{
    return _cliqueSA->cliques;
}


void CliqueSearcher::printCliques()
{
    list<set<unsigned>> cliques = _cliqueSA->cliques;
    list<set<unsigned>>::iterator currentClique = cliques.begin();
    set<unsigned>::iterator currentNode;

    for( ; currentClique != cliques.end(); currentClique++)
    {

        for(currentNode = currentClique->begin() ; currentNode != currentClique->end(); currentNode++)
        {
            cout << *currentNode << " ";
        }
        cout << endl;
    }
}

