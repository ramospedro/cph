#ifndef PERCOLATIONVERIFIER_H
#define PERCOLATIONVERIFIER_H

#include <set>
#include <list>
#include <algorithm>
#include <iostream>
#include <chrono>

using namespace std;

set<unsigned> *setUnion(set<unsigned> *set1, set<unsigned> *set2) {

    set<unsigned> *unionSet = new set<unsigned>;
    set_union(set1->begin(), set1->end(), set2->begin(), set2->end(), inserter(*unionSet, unionSet->end()));

    return unionSet;

}

set<unsigned> *setIntersection(set<unsigned> *set1, set<unsigned> *set2) {

    set<unsigned> *intersect = new set<unsigned>;
    set_intersection(set1->begin(),set1->end(),set2->begin(),set2->end(), inserter(*intersect,intersect->begin()));

    return intersect;

}

bool doTheseCliquesPercolate(set<unsigned> *clique1, set<unsigned> *clique2, unsigned numberOfNodesRequiered) {
    bool percolate;

    set<unsigned> *intersection = setIntersection(clique1, clique2);
    percolate = intersection->size() >= numberOfNodesRequiered;

    return percolate;
}

struct TNode {
    unsigned idClique;
    set<unsigned> *graphNodes;
    TNode *left;
    TNode *right;
    TNode *leftBrother;
    TNode *rightBrother;
    TNode *parent;
    bool visited;
    bool leaf;
};

struct TTree {
    TNode *root;
};

set<unsigned> *lastResult;

void printAllSetElements(set<unsigned> *graphNodes) {
    cout << endl;
    for (set<unsigned>::iterator i = graphNodes->begin(); i != graphNodes->end(); i++) {
        if (i != graphNodes->begin()) {
            cout << ", ";
        }
        cout << *i;
    }
    cout << endl;
}

bool recursivePercolationVerifier(set<unsigned> *cliqueNodes, unsigned idClique, TNode *currentTreeNode) {

    if (currentTreeNode->left != NULL) {
        if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode->left->graphNodes, cliqueNodes->size()-1)) {
            // if it's a leaf node, then it's a percolating clique and needs to be added to the lastResult list
            if (currentTreeNode->left->leaf) {
                // REVER quando tiver o visited não precisará mais, pois os de maior tamanho vão primeiro e evita isto
                if (cliqueNodes->size() == currentTreeNode->left->graphNodes->size()) {
                    if (currentTreeNode->left->idClique != idClique) {
                        lastResult->insert(currentTreeNode->left->idClique);
                    }
                }
            } else {
            // if it's not a leaf, the recursion tree keeps going until it has found one
                recursivePercolationVerifier(cliqueNodes, idClique, currentTreeNode->left);
            }
        }
    }
//cliqueNodes->size()-1)
    if (currentTreeNode->right != NULL) {
        if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode->right->graphNodes, cliqueNodes->size()-1)) {
            // if it's a leaf node, then it's a percolating clique and needs to be added to the lastResult list
            if (currentTreeNode->right->leaf) {
                // REVER quando tiver o visited não precisará mais, pois os de maior tamanho vão primeiro e evita isto
                if (cliqueNodes->size() == currentTreeNode->right->graphNodes->size()) {
                    if (currentTreeNode->right->idClique != idClique) {
                        lastResult->insert(currentTreeNode->right->idClique);
                    }
                }
            } else {
            // if it's not a leaf, the recursion tree keeps going until it has found one
                recursivePercolationVerifier(cliqueNodes, idClique, currentTreeNode->right);
            }
        }
    }

    return true;
}

set<unsigned> *getPercolatingCliques(set<unsigned> *cliqueNodes, unsigned idClique, TTree *cliqueTree) {
    lastResult = new set<unsigned>;
    recursivePercolationVerifier(cliqueNodes, idClique, cliqueTree->root);
    return lastResult;
}

TNode *newTNode() {

    TNode *tNode = new TNode;

    // memoria gasta a toa, revisar
    // quando é parent node não aproveita esse set criado aqui
    tNode->graphNodes = new set<unsigned>();

    tNode->idClique = 999999999;
    tNode->leaf = false;
    tNode->left = NULL;
    tNode->right = NULL;
    tNode->parent = NULL;

    return tNode;

}

TTree* buildCliqueTree(list<set<unsigned>*> *cliquesList) {

    list<TNode*> *nodesToBeAddedToTheTree = new list<TNode*>;

    TTree *cliqueTree = new TTree;

    list<set<unsigned>*>::iterator cliqueIterator;

    // processes all leafs and creates its parents
   for (int i = 0; cliqueIterator != cliquesList->end(); i+=2) {

        cliqueIterator = next(cliquesList->begin(), i);

        if (cliqueIterator != cliquesList->end()) {

            TNode *newCliqueLeft = newTNode();

            newCliqueLeft->idClique = i;
            newCliqueLeft->leaf = true;
            newCliqueLeft->graphNodes = *cliqueIterator;

            cliqueIterator = next(cliquesList->begin(), i+1);

            // last node odd
            if (cliqueIterator == cliquesList->end()) {
                //printAllSetElements(newCliqueLeft->graphNodes);
                // adds as the first element in order to mantain the tree as balanced as possible
                nodesToBeAddedToTheTree->push_front(newCliqueLeft);

            } else {
                TNode *newCliqueRight = newTNode();
                newCliqueRight->idClique = i+1;
                newCliqueRight->leaf = true;

                newCliqueRight->graphNodes = *cliqueIterator;

                TNode *newCliqueParent = newTNode();
                newCliqueParent->left = newCliqueLeft;
                newCliqueParent->right = newCliqueRight;

                newCliqueParent->graphNodes = setUnion(newCliqueLeft->graphNodes, newCliqueRight->graphNodes);

                nodesToBeAddedToTheTree->push_front(newCliqueParent);

            }
        }
    }

    // continues the tree building from the recent parent nodes created
    while (!nodesToBeAddedToTheTree->empty()) {

        TNode *newCliqueLeft = nodesToBeAddedToTheTree->front();
        nodesToBeAddedToTheTree->pop_front();


        // the root
        if (nodesToBeAddedToTheTree->empty()) {

            cliqueTree->root = newCliqueLeft;

        } else {

            TNode *newCliqueRight = nodesToBeAddedToTheTree->front();
            nodesToBeAddedToTheTree->pop_front();

            TNode *newCliqueParent = newTNode();
            newCliqueParent->left = newCliqueLeft;
            newCliqueParent->right = newCliqueRight;

            newCliqueParent->graphNodes = setUnion(newCliqueLeft->graphNodes, newCliqueRight->graphNodes);

            nodesToBeAddedToTheTree->push_back(newCliqueParent);
        }
    }

    return cliqueTree;

}





#endif // PERCOLATIONVERIFIER_H
