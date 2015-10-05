#ifndef CPH_H_INCLUDED
#define CPH_H_INCLUDED

#include <iostream>
#include "PercolationVerifier.h"
#include "CommunitiesFinder.h"
#include "../src/graph/largegraph.h"
#include "../src/clique/bronkerbosch.h"
#include <chrono>

bool compareSetSize (set<unsigned> *first, set<unsigned> *second) {
    return first->size() < second->size();
}

void printListSets(list<set<unsigned>*> *listSet) {

    cout << endl << "Print list set" << endl;
    unsigned i = 0;

    for (list<set<unsigned>*>::iterator currentSet = listSet->begin(); currentSet != listSet->end(); currentSet++, i++) {
        cout << endl << "Set " << i << endl;
        printAllSetElements(*currentSet);
        cout << endl;

    }


}

list<set<unsigned>*> *CreateCliquesListOrderedBySize(list<set<unsigned>> *cliquesList, unsigned minK) {
    list<set<unsigned>*> *cliquesOrderedBySize = new list<set<unsigned>*>;
    for (list<set<unsigned>>::iterator listIterator = cliquesList->begin(); listIterator != cliquesList->end(); listIterator++) {
        if (listIterator->size() >= minK) {
            cliquesOrderedBySize->push_back(&*listIterator);
        }

    }
    cliquesOrderedBySize->sort(compareSetSize);

    return cliquesOrderedBySize;
}

void detectCommunities(string netPath, unsigned minK, double alpha) {

    LargeGraph lg(netPath);
    BronKerbosch cl(&lg);

    srand(time(NULL));
    chrono::system_clock::time_point before;
    before = chrono::system_clock::now();

    cl.execute();

    long long int totalTimeClique = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();
    list<set<unsigned>*> *cliquesList = CreateCliquesListOrderedBySize(&cl.cliques, minK);
    long long int totalTimeConvertList = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();
    printListSets(cliquesList);
    before = chrono::system_clock::now();

    unordered_map<unsigned,set<unsigned>*> *communities = findCommunities(cliquesList, &lg, alpha);

    long long int totalTimeCPM = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    cout << endl << "Communities found" << endl << endl;

    /*unordered_map<unsigned,set<unsigned>*> *nodes = getCommunitiesOfEachNode(communities);

    for (unordered_map<unsigned,set<unsigned>*>::iterator itCom = communities->begin(); itCom != communities->end(); itCom++) {
        cout << endl << endl << itCom->first << " - ";
        for (set<unsigned>::iterator itSet = itCom->second->begin(); itSet != itCom->second->end(); itSet++) {
            cout << endl << *itSet + 1 << " edges: ";

            for (unordered_map<unsigned,LargeGraphEdge>::const_iterator edgeIt = lg.adjNodes[*itSet].begin(); edgeIt != lg.adjNodes[*itSet].end(); edgeIt++) {
                cout << edgeIt->second.targetNode+1 << ", ";
            }
            array<unsigned,2> deg = getCommunityDegree(itCom->second, &lg);
            cout << " int deg " << deg[0] << " - ext deg " << deg[1];


        }

        set<unsigned> *neighbors = getNeighborNodes(itCom, &lg);
        cout << endl << "neighbour nodes: ";
        for (set<unsigned>::iterator itSet = neighbors->begin(); itSet != neighbors->end(); itSet++) {
            cout << *itSet+1 << ", ";
        }

        set<unsigned> *neighborCommunities = getNeighborCommunities(neighbors, itCom, nodes);

        cout << endl << "neighbour communities: ";
        for (set<unsigned>::iterator itSet = neighborCommunities->begin(); itSet != neighborCommunities->end(); itSet++) {
            cout << *itSet << ", ";
        }

        cout.precision(15);
        cout << endl << endl << "Fitness " << std::fixed << getModuleFitness(itCom->second, &lg, alpha) << endl << endl;

    }*/

    /*for (unordered_map<unsigned,set<unsigned>*>::iterator currentNode = nodes->begin(); currentNode != nodes->end(); currentNode++) {
        cout << endl << "node " << currentNode->first+ 1 << " comunnities: ";
        for (set<unsigned>::iterator itSet = currentNode->second->begin(); itSet != currentNode->second->end(); itSet++) {
            cout << *itSet << ", ";
        }
    }*/

    cout << endl << endl;

    cout << "Communities found: " << communities->size();
    cout << endl << endl << endl;
    cout << "Time clique: " << totalTimeClique;
    cout << endl;
    cout << "Time CPM: " << totalTimeCPM;
    cout << endl;
    cout << "Time Convert List: " << totalTimeConvertList;




}

#endif // CPH_H_INCLUDED
