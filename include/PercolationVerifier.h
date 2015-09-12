#ifndef PERCOLATIONVERIFIER_H
#define PERCOLATIONVERIFIER_H

#include <set>
#include <list>
#include <algorithm>
#include <iostream>

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

bool doTheseCliquesPercolate(set<unsigned> &clique1, set<unsigned> &clique2, unsigned numberOfNodesRequired) {

    bool percolate;

    //set_intersection(clique1, clique2);
    set<unsigned> intersect;
    set_intersection(clique1.begin(),clique1.end(),clique2.begin(),clique2.end(), inserter(intersect,intersect.begin()));

    percolate = intersect.size() >= numberOfNodesRequired;

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

list<unsigned*> lastResult;

/*bool recursivePercolationVerifier(set<unsigned> &cliqueNodes, TNode &currentTreeNode) {

    if (currentTreeNode.left != NULL) {
        if (!currentTreeNode.left->visited) {
            if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode.left->graphNodes, 2)) {
                // if it's a leaf node, then it's a percolating clique and needs to be added to the lastResult list
                if (currentTreeNode.left->leaf) {
                    lastResult.push_back(&currentTreeNode.left->idClique);
                } else {
                // if it's not a leaf, the recursion tree keeps going until it has found one
                    recursivePercolationVerifier(cliqueNodes, *currentTreeNode.left);
                }
            }
        }
    }

    if (currentTreeNode.right != NULL) {
        if (!currentTreeNode.right->visited) {
            if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode.right->graphNodes, 2)) {
                // if it's a leaf node, then it's a percolating clique and needs to be added to the lastResult list
                if (currentTreeNode.right->leaf) {
                    lastResult.push_back(&currentTreeNode.right->idClique);
                } else {
                // if it's not a leaf, the recursion tree keeps going until it has found one
                    recursivePercolationVerifier(cliqueNodes, *currentTreeNode.right);
                }
            }
        }
    }

    currentTreeNode.visited = true;

    return true;
}

list<unsigned*> getPercolatingCliques(set<unsigned> *cliqueNodes, TTree *cliqueTree) {
    //lastResult = -1;
    recursivePercolationVerifier(*cliqueNodes, *cliqueTree->root);
    return lastResult;
}*/

TNode *newTNode() {

    TNode *tNode = new TNode;

    // memoria gasta a toa, revisar
    // quando é parent node não aproveita esse set criado aqui
    tNode->graphNodes = new set<unsigned>();

    tNode->idClique = -1;
    tNode->leaf = false;
    tNode->left = NULL;
    tNode->right = NULL;
    tNode->parent = NULL;

    return tNode;

}

TTree* buildCliqueTree(list<set<unsigned>> *cliquesList) {

    list<TNode*> *nodesToBeAddedToTheTree = new list<TNode*>;

    TTree *cliqueTree = new TTree;

    list<set<unsigned>>::iterator cliqueIterator;

    // processes all leafs and creates its parents
   for (int i = 0; cliqueIterator != cliquesList->end(); i+=2) {

        cliqueIterator = next(cliquesList->begin(), i);

        if (cliqueIterator != cliquesList->end()) {

            TNode *newCliqueLeft = newTNode();

            newCliqueLeft->idClique = i;
            newCliqueLeft->leaf = true;
            newCliqueLeft->graphNodes = &*cliqueIterator;

            cliqueIterator = next(cliquesList->begin(), i+1);

            // last node odd
            if (cliqueIterator == cliquesList->end()) {

                // adds as the first element in order to mantain the tree as balanced as possible
                nodesToBeAddedToTheTree->push_front(newCliqueLeft);

            } else {
                TNode *newCliqueRight = newTNode();
                newCliqueRight->idClique = i+1;
                newCliqueRight->leaf = true;

                newCliqueRight->graphNodes = &*cliqueIterator;

                TNode *newCliqueParent = newTNode();
                newCliqueParent->left = newCliqueLeft;
                newCliqueParent->right = newCliqueRight;

                newCliqueParent->graphNodes = setUnion(newCliqueLeft->graphNodes, newCliqueRight->graphNodes);

                nodesToBeAddedToTheTree->push_back(newCliqueParent);

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
