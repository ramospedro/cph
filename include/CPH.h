#ifndef CPH_H_INCLUDED
#define CPH_H_INCLUDED

#include <iostream>
#include "PercolationVerifier.h"
#include "CommunitiesFinder.h"
#include "../src/graph/largegraph.h"
#include "../src/clique/bronkerbosch.h"
#include "../src/clique/bkcliqueremove.h"
#include <chrono>
#include <fstream>
#include <cmath>

string getFileName(string filePath) {
    int positionLastSlash = filePath.find_last_of("/");
    string fileNameWithExtension = filePath.substr(positionLastSlash+1);
    int positionLastDot = fileNameWithExtension.find_last_of(".");
    int numberCharsExtension = fileNameWithExtension.length() - positionLastDot;
    int numberCharsFileName = fileNameWithExtension.length() - numberCharsExtension;

    cout << numberCharsFileName;
    string fileNameWithoutExtension = fileNameWithExtension.substr(0, numberCharsFileName);
    return fileNameWithoutExtension;
}

void writeResults(string cliqueStrategy, string instance, unsigned n, unsigned m, long long int time, unordered_map<unsigned,set<unsigned>*> *communities, string fileDirectory) {

    unsigned clusters = communities->size();
    string partitions = "{";
    string instanceName = getFileName(instance);
    string fileName = "cph_results_" + cliqueStrategy + "_" + instanceName + ".txt";

    bool firstPartition = true;

    for (unordered_map<unsigned,set<unsigned>*>::iterator itCom = communities->begin(); itCom != communities->end(); itCom++) {

        if (!firstPartition) {
            partitions += ", ";
        } else {
            firstPartition = false;
        }
        partitions += "[";
        bool first = true;
        for (set<unsigned>::iterator itSet = itCom->second->begin(); itSet != itCom->second->end(); itSet++) {

            if (!first) {
                partitions += ", ";
            } else {
                first = false;
            }
            partitions += to_string(*itSet + 1);
        }
        partitions += "]";
    }

    string result = cliqueStrategy + ", ";
    result += instanceName + ", " + to_string(n) + ", " + to_string(m) + ", " + to_string(time) + ", " + to_string(clusters);

    ofstream file;
    file.open(fileDirectory + fileName);
    file << result;
    file.close();

}

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

void detectCommunities(string netPath, unsigned minK, double alpha, unsigned cliqueAlg, unsigned maxIterationsModules, unsigned maxIterationsNodes, string resultsDirectory) {

    LargeGraph lg(netPath);
    list<set<unsigned>*> *cliquesList;
    string cliqueStrategy;

    BronKerbosch bkPivot(&lg);
    BronKerbosch bkCliqueRemove(&lg);

    srand(time(NULL));
    chrono::system_clock::time_point before;

    if (cliqueAlg == 1) {
        cliqueStrategy = "BKPivot";
        bkPivot.execute();
        cliquesList = CreateCliquesListOrderedBySize(&bkPivot.cliques, minK);
    } else if (cliqueAlg == 2 ) {
        cliqueStrategy = "BKCliqueRemove";
        bkCliqueRemove.execute();
        cliquesList = CreateCliquesListOrderedBySize(&bkCliqueRemove.cliques, minK);
    } else {
        exit(0);
    }

    before = chrono::system_clock::now();

    unordered_map<unsigned,set<unsigned>*> *communities = findCommunities(cliquesList, &lg, alpha, maxIterationsModules, maxIterationsNodes);

    long long int totalTimeCPM = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    writeResults(cliqueStrategy, netPath, lg.numberOfNodes, lg.numberOfEdges, totalTimeCPM, communities, resultsDirectory);

    /*cout << endl << "Communities found" << endl << endl;

    unordered_map<unsigned,set<unsigned>*> *nodes = getCommunitiesOfEachNode(communities);

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


}



#endif // CPH_H_INCLUDED
