#include <iostream>
#include "include/PercolationVerifier.h"


using namespace std;

/*void test1() {
    set<unsigned> nodes;
    nodes.insert(1);
    nodes.insert(2);
    nodes.insert(3);
    nodes.insert(4);

    set<unsigned> clique1;
    clique1.insert(1);
    clique1.insert(2);
    clique1.insert(3);

    set<unsigned> clique2;
    clique2.insert(2);
    clique2.insert(3);
    clique2.insert(4);

    TTree cliqueTree;
    TNode r;
    TNode c1;
    TNode c2;

    r.left = &c1;
    r.right = &c2;
    cliqueTree.root = &r;

    r.graphNodes = nodes;
    r.leaf = false;

    c1.graphNodes = clique1;
    c1.leaf = true;
    c1.idClique = 100;

    c2.graphNodes = clique2;
    c2.leaf = true;
    c2.idClique = 200;


    list<unsigned*> result = getPercolatingCliques(&clique1, &cliqueTree);
    cout << *result.front();
    cout << *result.back();
}*/

void test2() {

    set<unsigned> clique1;
    clique1.insert(1);
    clique1.insert(2);
    clique1.insert(3);

    set<unsigned> clique2;
    clique2.insert(2);
    clique2.insert(3);
    clique2.insert(4);

    set<unsigned> clique3;
    clique2.insert(2);
    clique2.insert(3);
    clique2.insert(4);

    set<unsigned> clique4;
    clique2.insert(2);
    clique2.insert(3);
    clique2.insert(4);


    list<set<unsigned>> cliquesList;
    cliquesList.push_back(clique1);
    cliquesList.push_back(clique2);
    cliquesList.push_back(clique3);
    cliquesList.push_back(clique4);

    buildCliqueTree(&cliquesList);

}


struct TElement {

    TElement *next;
    unsigned id;

};

struct TCollection {

    TElement *first;

};

void learn() {

    TCollection *col = new TCollection;

    col->first = new TElement;
    col->first->id = 0;

    TElement *nav = col->first;

    for (int i = 1; i < 10; i++) {

        TElement *newElement = new TElement;
        newElement->id = i;

        nav->next = newElement;
        nav = newElement;

    }

    nav = col->first;

    for (int i = 0; i < 10; i++) {

        cout << nav->id;
        nav = nav->next;

    }


}


int main()
{

    test2();
    return 0;
}
