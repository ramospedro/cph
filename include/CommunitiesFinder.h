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
#include "math.h"
#include <chrono>

using namespace std;

set<unsigned> *getNeighborNodes(unordered_map<unsigned,set<unsigned>*>::iterator community, LargeGraph *graph) {

    set<unsigned> *neighborNodes = new set<unsigned>;

    // iterates over each node from the given community
    for (set<unsigned>::iterator node = community->second->begin(); node != community->second->end(); node++) {

        for (unordered_map<unsigned,LargeGraphEdge>::const_iterator edgeIt = graph->adjNodes[*node].begin(); edgeIt != graph->adjNodes[*node].end(); edgeIt++) {
            if (community->second->find(edgeIt->second.targetNode) == community->second->end()) {
                if (neighborNodes->find(edgeIt->second.targetNode) == neighborNodes->end()) {
                        neighborNodes->insert(edgeIt->second.targetNode);
                }
            }
        }

    }

    return neighborNodes;
}

set<unsigned> *getNeighborCommunities(set<unsigned> *neighborNodes, unordered_map<unsigned,set<unsigned>*>::iterator community, unordered_map<unsigned,set<unsigned>*> *nodes) {

    set<unsigned> *neighborCommunities = new set<unsigned>;

    // iterates over each node from the given community
    for (set<unsigned>::iterator node = neighborNodes->begin(); node != neighborNodes->end(); node++) {
        unordered_map<unsigned,set<unsigned>*>::iterator nodeIterator = nodes->find(*node);

        // not all nodes bolong to a community
        if (nodeIterator != nodes->end()) {
            // iterates over the communities on witch the node belongs in
            for (set<unsigned>::iterator communityIterator = nodeIterator->second->begin(); communityIterator != nodeIterator->second->end(); communityIterator++) {
                if (*communityIterator != community->first) {
                    if (neighborCommunities->find(*communityIterator) == neighborCommunities->end()) {
                        neighborCommunities->insert(*communityIterator);
                    }
                }
            }
        }
    }

    return neighborCommunities;
}

unordered_map<unsigned,set<unsigned>*> *getCommunitiesOfEachNode(unordered_map<unsigned,set<unsigned>*> *communities) {

    unordered_map<unsigned,set<unsigned>*> *nodes = new unordered_map<unsigned,set<unsigned>*>;

    for (unordered_map<unsigned,set<unsigned>*>::iterator currentCommunity = communities->begin(); currentCommunity != communities->end(); currentCommunity++) {

        for (set<unsigned>::iterator currentNode = currentCommunity->second->begin(); currentNode != currentCommunity->second->end(); currentNode++) {

            unordered_map<unsigned,set<unsigned>*>::iterator nodeIterator = nodes->find(*currentNode);

            if (nodeIterator == nodes->end()) {
                set<unsigned> *newSet = new set<unsigned>;
                newSet->insert(currentCommunity->first);
                nodes->insert({*currentNode, newSet});
            } else {
                nodeIterator->second->insert(currentCommunity->first);
            }

        }

    }

    return nodes;
}

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

    return degrees;
}

double getModuleFitness(set<unsigned> *module, LargeGraph *graph, double alpha) {

    array<unsigned,2> moduleDegrees = getCommunityDegree(module, graph);
    unsigned moduleInternalDegree = moduleDegrees[0];
    unsigned moduleExternalDegree = moduleDegrees[1];

    double moduleFitness = (double) moduleInternalDegree/pow(moduleInternalDegree+moduleExternalDegree, alpha);

    return moduleFitness;
}

bool isTheFirstModuleFitnessBetterThanOrEqualTheSecond(set<unsigned> *firstModule, set<unsigned> *secondModule, LargeGraph *graph, double alpha) {

    bool betterThanOrEqual = false;

    double firstModuleFitness = getModuleFitness(firstModule, graph, alpha);
    double secondModuleFitness = getModuleFitness(secondModule, graph, alpha);

    //out << endl << endl << "fitness first " << firstModuleFitness << "  fit sec " << secondModuleFitness << endl << endl;
    if (firstModuleFitness >= secondModuleFitness) {
        betterThanOrEqual = true;
    }

    return betterThanOrEqual;
}



// if it's not better than or equal, returns null
set<unsigned> *getMergedModulesIfBetterOrEqualFitness(set<unsigned> *firstModule, set<unsigned> *secondModule, LargeGraph *graph, double alpha) {

    set<unsigned> *mergedModules = setUnion(firstModule, secondModule);

    bool betterThanOrEqual = isTheFirstModuleFitnessBetterThanOrEqualTheSecond(mergedModules, firstModule, graph, alpha);

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

        if (communityToBeMergedIntoTheCurrentCommunity == communities->end()) {

            cout << "Não vai cair aqui, não vai cair aqui, não vai cair aqui..." << idCliqueToBeProcessed;
            exit(0);
        }

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

void nodesUnion(unordered_map<unsigned,set<unsigned>*> *communities, LargeGraph *graph, unsigned maxIterations, double alpha) {

    for (unsigned i = 0; i < maxIterations; i++) {

        bool iterationGeneratedAlterations = false;

        list<pair<unsigned,set<unsigned>*>> *communitiesToBeProcessed = new list<pair<unsigned,set<unsigned>*>>;

        for (unordered_map<unsigned,set<unsigned>*>::iterator currentCommunity = communities->begin(); currentCommunity != communities->end(); currentCommunity++) {
            communitiesToBeProcessed->push_back(*currentCommunity);
        }

        communitiesToBeProcessed->sort(compareSetSizeHighToLow);

        for(list<pair<unsigned,set<unsigned>*>>::iterator processedCommunity = communitiesToBeProcessed->begin(); processedCommunity != communitiesToBeProcessed->end(); processedCommunity++) {

            unordered_map<unsigned,set<unsigned>*>::iterator community = communities->find(processedCommunity->first);

            // if the module hasn't been merged yet
            if (community != communities->end()) {

                set<unsigned> *neighbourNodes = getNeighborNodes(community, graph);

                for(set<unsigned>::iterator idNeighbourNode = neighbourNodes->begin(); idNeighbourNode != neighbourNodes->end(); idNeighbourNode++) {

                    set<unsigned> *possibleNodeToBeMerged = new set<unsigned>;
                    possibleNodeToBeMerged->insert(*idNeighbourNode);
                    set<unsigned> *possibleMergedCommunity = getMergedModulesIfBetterOrEqualFitness(community->second, possibleNodeToBeMerged, graph, alpha);

                    if (possibleMergedCommunity != NULL) {
                        community->second = possibleMergedCommunity;
                        iterationGeneratedAlterations = true;
                    }

                }
            }
        }

        if (!iterationGeneratedAlterations) {
            break;
        }
    }

}

void modulesUnion(unordered_map<unsigned,set<unsigned>*> *communities, LargeGraph *graph, unsigned maxIterations, double alpha) {

    srand(time(NULL));
    chrono::system_clock::time_point before;
    chrono::system_clock::time_point before2;
    chrono::system_clock::time_point before3;

    long long int totalTimeGetCommunitiesEachNode = 0;
    long long int totalTimeGetNeighbourNodes = 0;
    long long int totalTimeGetNeighbourCommunities = 0;
    long long int totalTimeForNeighbour = 0;
    long long int totalTimeGetMerged = 0;
    long long int totalTimeFindFor = 0;
    long long int totalTimeIfFor = 0;

    long long int tempTime = 0;

    int totalTimesFor = 0;

    for (unsigned i = 0; i < maxIterations; i++) {

        bool iterationGeneratedAlterations = false;

        list<pair<unsigned,set<unsigned>*>> *communitiesToBeProcessed = new list<pair<unsigned,set<unsigned>*>>;

        for (unordered_map<unsigned,set<unsigned>*>::iterator currentCommunity = communities->begin(); currentCommunity != communities->end(); currentCommunity++) {
            communitiesToBeProcessed->push_back(*currentCommunity);
        }

        communitiesToBeProcessed->sort(compareSetSizeHighToLow);


        for(list<pair<unsigned,set<unsigned>*>>::iterator processedCommunity = communitiesToBeProcessed->begin(); processedCommunity != communitiesToBeProcessed->end(); processedCommunity++) {

            unordered_map<unsigned,set<unsigned>*>::iterator community = communities->find(processedCommunity->first);

            // if the module hasn't been merged yet
            if (community != communities->end()) {

                unordered_map<unsigned,set<unsigned>*> *nodes = getCommunitiesOfEachNode(communities);
                set<unsigned> *neighbourNodes = getNeighborNodes(community, graph);
                set<unsigned> *neighbourCommunities = getNeighborCommunities(neighbourNodes, community, nodes);

                before = chrono::system_clock::now();
                for(set<unsigned>::iterator idNeighbourCommunity = neighbourCommunities->begin(); idNeighbourCommunity != neighbourCommunities->end(); idNeighbourCommunity++) {

                    unordered_map<unsigned,set<unsigned>*>::iterator communityToBeMerged = communities->find(*idNeighbourCommunity);

                    if (communityToBeMerged != communities->end()) {

                        set<unsigned> *possibleMergedCommunity = getMergedModulesIfBetterOrEqualFitness(community->second, communityToBeMerged->second, graph, alpha);

                        if (possibleMergedCommunity != NULL) {
                            community->second = possibleMergedCommunity;
                            communities->erase(communityToBeMerged);
                            iterationGeneratedAlterations = true;
                        }
                    }

                }
                tempTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();
                totalTimeForNeighbour += tempTime;
            }
        }

        if (!iterationGeneratedAlterations) {
            break;
        }
    }

    cout << endl << "totalTimeForNeighbour: " << totalTimeForNeighbour;
}

unordered_map<unsigned,set<unsigned>*> *findCommunities(list<set<unsigned>*> *cliquesList, LargeGraph *graph, double alpha) {

    unordered_map<unsigned,set<unsigned>*> *communities = new unordered_map<unsigned,set<unsigned>*>;

    srand(time(NULL));
    chrono::system_clock::time_point before;
    before = chrono::system_clock::now();

    TTree *cliqueTree = buildCliqueTree(cliquesList);

    long long int totalTimeBuildTree = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();

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
        cliquePercolations = getPercolatingCliques(*cliqueIterator, i, cliqueTree);
        //cout << "Percolations " << i;
        //printAllSetElements(cliquePercolations);

        percolations->push_back(cliquePercolations);
    }

    long long int totalTimeGetPercolations = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();
    createInitialModules(communities, percolations);
    long long int totalTimeCreateCliqueModules = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();
    modulesUnion(communities, graph, 1, alpha);
    long long int totalTimeModulesUnion = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();
    nodesUnion(communities, graph, 1, alpha);
    long long int totalTimeNodesUnion = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    cout << endl << "Time spent building the CliqueTree: " << totalTimeBuildTree;
    cout << endl << "Time spent getting the Percolations: " << totalTimeGetPercolations;
    cout << endl << "Time spent creating the clique modules: " << totalTimeCreateCliqueModules;
    cout << endl << "Time spent in modules union: " << totalTimeModulesUnion;
    cout << endl << "Time spent in nodes union: " << totalTimeNodesUnion << endl << endl;

    return communities;
}





#endif // COMMUNITIESFINDER_H_INCLUDED
