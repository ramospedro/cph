#ifndef COMMUNITIESFINDER_H_INCLUDED
#define COMMUNITIESFINDER_H_INCLUDED

#include <set>
#include <list>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "PercolationVerifier.h"
#include <vector>

using namespace std;

void recursiveInitialModulesCreation
    (unordered_map<unsigned,set<unsigned>*> *communities, vector<set<unsigned>*> *percolations, unordered_map<unsigned,set<unsigned>*>::iterator currentCommunity, set<unsigned> *cliquesInTheCurrentCommunity, unsigned idCliqueToBeProcessed) {

    cliquesInTheCurrentCommunity->insert(idCliqueToBeProcessed);

    if (currentCommunity->first != idCliqueToBeProcessed) {

        unordered_map<unsigned,set<unsigned>*>::iterator communityToBeMergedIntoTheCurrentCommunity = communities->find(idCliqueToBeProcessed);

        currentCommunity->second = setUnion(currentCommunity->second, communityToBeMergedIntoTheCurrentCommunity->second);

        communities->erase(communityToBeMergedIntoTheCurrentCommunity);
    }

    set<unsigned>* percolationsFromTheProcessedClique = percolations->at(idCliqueToBeProcessed);
    for (set<unsigned>::iterator cliqueIterator = percolationsFromTheProcessedClique->begin(); cliqueIterator != percolationsFromTheProcessedClique->end(); cliqueIterator++) {
        // recursivelly calls the method only if the clique hasn't been merged into this community yet
        if (cliquesInTheCurrentCommunity->find(*cliqueIterator) == cliquesInTheCurrentCommunity->end()){
            recursiveInitialModulesCreation(communities, percolations, currentCommunity, cliquesInTheCurrentCommunity, *cliqueIterator);
        }

    }

}

unordered_map<unsigned,set<unsigned>*> *createInitialModules(unordered_map<unsigned,set<unsigned>*> *communities, vector<set<unsigned>*> *percolations) {

    unsigned communitiesCount = communities->size();


    // REVER se for ficar assim, ver pelo menos a ordem das comunidades que está de tras pra frente, porém o contador começa do 0
    for (int i = 0; i < communitiesCount; i++) {
        unordered_map<unsigned,set<unsigned>*>::iterator communityIterator = communities->find(i);

        // not all communities will be found, since they're being removed inside the recursive method
        if (communityIterator != communities->end()) {
            recursiveInitialModulesCreation(communities, percolations, communityIterator, new set<unsigned>, communityIterator->first);
        }
    }

    return communities;

}

unordered_map<unsigned,set<unsigned>*> *findCommunities(list<set<unsigned>*> *cliquesList) {

    unordered_map<unsigned,set<unsigned>*> *communities = new unordered_map<unsigned,set<unsigned>*>;
    TTree *cliqueTree = buildCliqueTree(cliquesList);
    vector<set<unsigned>*> *percolations = new vector<set<unsigned>*>;

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
        set<unsigned> *cliquePercolations = getPercolatingCliques(*cliqueIterator, i, cliqueTree);
        percolations->push_back(cliquePercolations);
    }

    createInitialModules(communities, percolations);

    return communities;
}





#endif // COMMUNITIESFINDER_H_INCLUDED
