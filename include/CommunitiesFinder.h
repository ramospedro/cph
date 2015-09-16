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


unordered_map<unsigned,set<unsigned>*> *findCommunities(list<set<unsigned>*> *cliquesList) {

    unordered_map<unsigned,set<unsigned>*> *communities = new unordered_map<unsigned,set<unsigned>*>;
    TTree *cliqueTree = buildCliqueTree(cliquesList);
    vector<set<unsigned>*> *percolations;

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

    return communities;
}

void recursiveInitialModulesCreation
    (unordered_map<unsigned,set<unsigned>*> *communities, vector<set<unsigned>*> *percolations, set<unsigned> *currentCommunity, set<unsigned> *cliquesInTheCurrentCommunity, unsigned idCliqueToBeProcessed) {

    cliquesInTheCurrentCommunity->insert(idCliqueToBeProcessed);


    unordered_map<unsigned,set<unsigned>*>::iterator communityToBeMergedIntoTheCurrentCommunity = communities->find(idCliqueToBeProcessed);
    currentCommunity = setUnion(currentCommunity, communityToBeMergedIntoTheCurrentCommunity->second);
    communities->erase(communityToBeMergedIntoTheCurrentCommunity);

    set<unsigned>* percolationsFromTheProcessedClique = percolations->at(idCliqueToBeProcessed);
    for (set<unsigned>::iterator cliqueIterator = percolationsFromTheProcessedClique->begin(); cliqueIterator != percolationsFromTheProcessedClique->end(); cliqueIterator++) {

        // recursivelly calls the method only if the clique hasn't been merged into this community yet
        if (cliquesInTheCurrentCommunity->find(*cliqueIterator) != cliquesInTheCurrentCommunity->end()){
            recursiveInitialModulesCreation(communities, percolations, currentCommunity, cliquesInTheCurrentCommunity, *cliqueIterator);
        }

    }

}

void createInitialModules(unordered_map<unsigned,set<unsigned>*> *communities, vector<set<unsigned>*> *percolations) {

    for (unordered_map<unsigned,set<unsigned>*>::iterator communityIterator; communityIterator != communities->end(); communityIterator++) {
        recursiveInitialModulesCreation(communities, percolations, communityIterator->second, new set<unsigned>, communityIterator->first);
    }

}




#endif // COMMUNITIESFINDER_H_INCLUDED
