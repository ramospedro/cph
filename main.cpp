#include <iostream>
#include "include/PercolationVerifier.h"
#include "include/CommunitiesFinder.h"

using namespace std;

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

    findCommunities(cliquesList);

    //TTree *cliqueTree = buildCliqueTree(cliquesList);

    /*set<unsigned> *graphNodes = cliqueTree->root->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }

    cout << endl << endl;
    graphNodes = cliqueTree->root->left->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }
    cout << endl;
    graphNodes = cliqueTree->root->left->left->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }

    cout << endl;
    graphNodes = cliqueTree->root->left->right->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }

    cout << endl << endl;
    graphNodes = cliqueTree->root->right->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }
    cout << endl << endl;
    graphNodes = cliqueTree->root->right->left->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }

    cout << endl << endl;
    graphNodes = cliqueTree->root->right->right->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }

    cout << endl;
    graphNodes = cliqueTree->root->right->right->right->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }
    cout << endl;
    graphNodes = cliqueTree->root->right->right->left->graphNodes;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        cout << *i << ", ";
    }
*/
    /*cout << endl << endl << "Percolação" << endl << endl;
    list<unsigned> *percolation = getPercolatingCliques(clique4, 3, cliqueTree);

    for (list<unsigned>::iterator it = percolation->begin(); it != percolation->end(); it++) {

        cout << *it;

    }*/

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

void learn2() {

    TNode *nodeA = newTNode();
    TNode *nodeB = newTNode();

    set<unsigned> *setA = new set<unsigned>;
    set<unsigned> *setB = new set<unsigned>;

    setA->insert(1);
    setA->insert(2);
    setA->insert(3);
    setA->insert(4);

    setB->insert(3);
    setB->insert(4);
    setB->insert(5);
    setB->insert(6);

    set<unsigned> *uni = setUnion(setA, setB);
    cout << uni->size() << endl;
    set<unsigned> *inter = setIntersection(setA, setB);
    cout << inter->size() << endl << endl;

    set<unsigned> *unionSet = new set<unsigned>;
    set_union(setA->begin(), setA->end(), setB->begin(), setB->end(), inserter(*unionSet, unionSet->end()));
    cout << unionSet->size() << endl;

    set<unsigned> *intersect = new set<unsigned>;
    set_intersection(setA->begin(),setA->end(),setB->begin(),setB->end(), inserter(*intersect,intersect->begin()));
    cout << intersect->size();
}

void learn3() {

    list<unsigned> *listA = new list<unsigned>;
    listA->push_back(1);
    listA->push_back(2);
    listA->push_back(3);

    list<unsigned>::iterator it;


    for (int i = 0; it != listA->end(); i++) {

        it = next(listA->begin(), i);

        if (it != listA->end()) {
            cout << *it << endl;
        }
    }


}

int main()
{

    test1();
    return 0;
}
