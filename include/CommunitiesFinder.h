#ifndef COMMUNITIESFINDER_H_INCLUDED
#define COMMUNITIESFINDER_H_INCLUDED

#include <set>
#include <list>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "PercolationVerifier.h"
#include <vector>
#include "../src/graph/largegraph.h"

using namespace std;

bool compareSetSizeHighToLow (pair<unsigned,set<unsigned>*> first, pair<unsigned,set<unsigned>*> second) {
    return first.second->size() > second.second->size();
}

//[0] internal degree sum [1] external degree sum
array<unsigned,2> getCommunityDegree(set<unsigned> *community, LargeGraph *graph) {

    array<unsigned,2> degrees {0, 0};

    for (set<unsigned>::iterator nodeIt = community->begin(); nodeIt != community->end(); nodeIt++) {
        for (unordered_map<unsigned,LargeGraphEdge>::const_iterator edgeIt = graph->adjNodes[*nodeIt].begin(); edgeIt != graph->adjNodes[*nodeIt].end(); edgeIt++) {
            if (community->find(edgeIt->second.targetNode) != community->end()) {
                degrees[0]++;
            } else {
                degrees[1]++;
            }
        }
    }

    // internal edges are being counted twice
    degrees[0] = degrees[0]/2;
    return degrees;
}

double getModuleFitness(set<unsigned> *module, LargeGraph *graph) {

    array<unsigned,2> moduleDegrees = getCommunityDegree(module, graph);
    unsigned moduleInternalDegree = moduleDegrees[0];
    unsigned moduleExternalDegree = moduleDegrees[1];

    double moduleFitness = (double) moduleInternalDegree/(moduleInternalDegree+moduleExternalDegree);

    return moduleFitness;
}

bool isTheFirstModuleFitnessBetterThanOrEqualTheSecond(set<unsigned> *firstModule, set<unsigned> *secondModule, LargeGraph *graph) {

    bool betterThanOrEqual = false;

    double firstModuleFitness = getModuleFitness(firstModule, graph);
    double secondModuleFitness = getModuleFitness(secondModule, graph);

    cout << endl << endl << "fitness first " << firstModuleFitness << "  fit sec " << secondModuleFitness << endl << endl;
    if (firstModuleFitness >= secondModuleFitness) {
        betterThanOrEqual = true;
    }

    return betterThanOrEqual;
}





// if it's not better than or equal, returns null
set<unsigned> *getMergedModulesIfBetterOrEqualFitness(set<unsigned> *firstModule, set<unsigned> *secondModule, LargeGraph *graph) {

    set<unsigned> *mergedModules = setUnion(firstModule, secondModule);

    bool betterThanOrEqual = isTheFirstModuleFitnessBetterThanOrEqualTheSecond(mergedModules, firstModule, graph);

    if (!betterThanOrEqual) {
        mergedModules = NULL;
    }

    return mergedModules;
}


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
        // recursivelly calls the method only if the clique hasn't been merged into any community yet

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

void modulesUnion(unordered_map<unsigned,set<unsigned>*> *communities, LargeGraph *graph) {

    list<pair<unsigned,set<unsigned>*>> *communitiesToBeProcessed = new list<pair<unsigned,set<unsigned>*>>;

    for (unordered_map<unsigned,set<unsigned>*>::iterator currentCommunity = communities->begin(); currentCommunity != communities->end(); currentCommunity++) {
        communitiesToBeProcessed->push_back(*currentCommunity);
    }

    communitiesToBeProcessed->sort(compareSetSizeHighToLow);

    for(list<pair<unsigned,set<unsigned>*>>::iterator processedCommunity = communitiesToBeProcessed->begin(); processedCommunity != communitiesToBeProcessed->end(); processedCommunity++) {

        unordered_map<unsigned,set<unsigned>*>::iterator community = communities->find(processedCommunity->first);

        // if the module hasn't been merged yet
        if (community != communities->end()) {

            for(list<pair<unsigned,set<unsigned>*>>::iterator currentCommunity = communitiesToBeProcessed->begin(); currentCommunity != communitiesToBeProcessed->end(); currentCommunity++) {

                if (processedCommunity->first != currentCommunity->first) {

                    unordered_map<unsigned,set<unsigned>*>::iterator communityToBeMerged = communities->find(currentCommunity->first);

                    if (communityToBeMerged != communities->end()) {

                        set<unsigned> *possibleMergedCommunity = getMergedModulesIfBetterOrEqualFitness(community->second, communityToBeMerged->second, graph);

                        if (possibleMergedCommunity != NULL) {
                            community->second = possibleMergedCommunity;
                            communities->erase(communityToBeMerged);

                        }

                    }

                }

            }

        }

    }


}

unordered_map<unsigned,set<unsigned>*> *findCommunities(list<set<unsigned>*> *cliquesList, unsigned maxK, LargeGraph *graph) {

    unordered_map<unsigned,set<unsigned>*> *communities = new unordered_map<unsigned,set<unsigned>*>;

    srand(time(NULL));
    chrono::system_clock::time_point before;
    before = chrono::system_clock::now();

    TTree *cliqueTree = buildCliqueTree(cliquesList);

    long long int totalTimeBuildTree = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    cout << endl << endl << "Time spent building the CliqueTree: " << totalTimeBuildTree << endl << endl;

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

        set<unsigned> *cliquePercolations;
        // gets the percolations for this clique
        cliquePercolations = getPercolatingCliques(*cliqueIterator, i, cliqueTree, maxK);
                    cout << "Percolations " << i;
            printAllSetElements(cliquePercolations);
        /*
        if ((*cliqueIterator)->size() <= maxK) {
            cliquePercolations = getPercolatingCliques(*cliqueIterator, i, cliqueTree, maxK);
            cout << "Percolations " << i;
            printAllSetElements(cliquePercolations);
        } else {
            cliquePercolations = new set<unsigned>;
        }
        */


        percolations->push_back(cliquePercolations);
    }

    createInitialModules(communities, percolations);

    modulesUnion(communities, graph);

    return communities;
}





#endif // COMMUNITIESFINDER_H_INCLUDED
