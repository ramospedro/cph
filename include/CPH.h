#ifndef CPH_H_INCLUDED
#define CPH_H_INCLUDED

#include <iostream>
#include "PercolationVerifier.h"
#include "CommunitiesFinder.h"
#include "../src/graph/largegraph.h"
#include "../src/clique/bronkerbosh.h"
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

void detectCommunities(string netPath, unsigned minK, unsigned maxK) {

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

    unordered_map<unsigned,set<unsigned>*> *communities = findCommunities(cliquesList, maxK);

    long long int totalTimeCPM = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    cout << endl << "Communities found" << endl << endl;
    for (unordered_map<unsigned,set<unsigned>*>::iterator itCom = communities->begin(); itCom != communities->end(); itCom++) {
        cout << itCom->first << " - ";
        for (set<unsigned>::iterator itSet = itCom->second->begin(); itSet != itCom->second->end(); itSet++) {
            cout << *itSet + 1 << " ";
        }

        cout << endl << endl;
    }

    cout << "Communities found: " << communities->size();
    cout << endl << endl << endl;
    cout << "Time clique: " << totalTimeClique;
    cout << endl;
    cout << "Time CPM: " << totalTimeCPM;
    cout << endl;
    cout << "Time Convert List: " << totalTimeConvertList;

}

#endif // CPH_H_INCLUDED
