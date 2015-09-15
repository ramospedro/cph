#ifndef COMMUNITIESFINDER_H_INCLUDED
#define COMMUNITIESFINDER_H_INCLUDED

#include <set>
#include <list>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "PercolationVerifier.h"

using namespace std;

unordered_map<unsigned,set<unsigned>*> *findCommunities(list<set<unsigned>*> *cliquesList) {

    unordered_map<unsigned,set<unsigned>*> *communities = new unordered_map<unsigned,set<unsigned>*>;
    TTree *cliqueTree = buildCliqueTree(cliquesList);
    list<list<unsigned>*> *percolations = new list<list<unsigned>*>;

    // initializes the commnunities
    // at the beginning, each clique is a different community
    // note that the commnunities index will be the as the clique index
    unsigned i = 0;
    for (list<set<unsigned>*>::iterator cliqueIterator = cliquesList->begin(); cliqueIterator != cliquesList->end(); cliqueIterator++, i++) {

        set<unsigned> *newCommunity = new set<unsigned>;
        for (set<unsigned>::iterator nodeIterator = (*cliqueIterator)->begin(); nodeIterator != (*cliqueIterator)->end(); nodeIterator++) {
            newCommunity->insert(*nodeIterator);
        }
        communities->insert({i, newCommunity});

        // gets the percolations for this clique
        list<unsigned> *cliquePercolations = getPercolatingCliques(*cliqueIterator, i, cliqueTree);
        percolations->push_back(cliquePercolations);
    }

    i = 0;

    for (list<list<unsigned>*>::iterator cliqueIterator = percolations->begin(); cliqueIterator != percolations->end(); cliqueIterator++, i++) {
        cout << endl;
        for (list<unsigned>::iterator nodeIterator = (*cliqueIterator)->begin(); nodeIterator != (*cliqueIterator)->end(); nodeIterator++) {
            cout << endl << *nodeIterator;
        }
    }

    return communities;
}

//void mergeCommunities()




#endif // COMMUNITIESFINDER_H_INCLUDED
