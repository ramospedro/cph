#include <iostream>
#include "include/PercolationVerifier.h"
#include "include/CommunitiesFinder.h"
#include "src/graph/largegraph.h"
#include "src/clique/bronkerbosh.h"
#include <chrono>

using namespace std;

list<set<unsigned>*> *ConvertList(list<set<unsigned>> *listToBeConverted) {

    list<set<unsigned>*> *newList = new list<set<unsigned>*>;

    for (list<set<unsigned>>::iterator listIterator = listToBeConverted->begin(); listIterator != listToBeConverted->end(); listIterator++) {
        if ((&*listIterator)->size() > 2) {
            newList->push_back(&*listIterator);
        }

    }

    return newList;
}

void test2() {

    LargeGraph lg("/home/pedro/Dropbox/2015-DeteccaoComunidadesSobreposicao (1)/desenvolvimento/instances/normalizadas/email.net");
    BronKerbosch cl(&lg);


    srand(time(NULL));
    chrono::system_clock::time_point before;
    before = chrono::system_clock::now();

    cl.execute();

    long long int totalTimeClique = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();
    list<set<unsigned>*> *cliquesList = ConvertList(&cl.cliques);
    long long int totalTimeConvertList = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    before = chrono::system_clock::now();

    unordered_map<unsigned,set<unsigned>*> *communities = findCommunities(cliquesList);

    long long int totalTimeCPM = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-before).count();

    cout << endl << "Communities found" << endl << endl;
    for (unordered_map<unsigned,set<unsigned>*>::iterator itCom = communities->begin(); itCom != communities->end(); itCom++) {
        cout << itCom->first << " - ";
        for (set<unsigned>::iterator itSet = itCom->second->begin(); itSet != itCom->second->end(); itSet++) {
            cout << *itSet << " ";
        }

        cout << endl << endl;
    }

    cout << endl << endl << endl;
    cout << "Time clique: " << totalTimeClique;
    cout << endl;
    cout << "Time CPM: " << totalTimeCPM;
    cout << endl;
    cout << "Time Convert List" << totalTimeConvertList;
}



void test1() {

    set<unsigned> *clique1 = new set<unsigned>;
    clique1->insert(1);
    clique1->insert(2);
    clique1->insert(3);

    set<unsigned> *clique2 = new set<unsigned>;
    clique2->insert(2);
    clique2->insert(3);
    clique2->insert(4);

    set<unsigned> *clique3 = new set<unsigned>;
    clique3->insert(4);
    clique3->insert(5);
    clique3->insert(6);

    set<unsigned> *clique4 = new set<unsigned>;
    clique4->insert(5);
    clique4->insert(6);
    clique4->insert(7);

    set<unsigned> *clique5 = new set<unsigned>;
    clique5->insert(6);
    clique5->insert(7);
    clique5->insert(8);

    list<set<unsigned>*> *cliquesList = new list<set<unsigned>*>;
    cliquesList->push_back(clique1);
    cliquesList->push_back(clique2);
    cliquesList->push_back(clique3);
    cliquesList->push_back(clique4);
    cliquesList->push_back(clique5);

    unordered_map<unsigned,set<unsigned>*> *communities = findCommunities(cliquesList);


    cout << endl << "Communities found" << endl << endl;
    for (unordered_map<unsigned,set<unsigned>*>::iterator itCom = communities->begin(); itCom != communities->end(); itCom++) {
        cout << itCom->first << " - ";
        for (set<unsigned>::iterator itSet = itCom->second->begin(); itSet != itCom->second->end(); itSet++) {
            cout << *itSet << " ";
        }

        cout << endl << endl;
    }

}

int main()
{

    test2();
    return 0;
}
