#ifndef PERCOLATIONVERIFIER_H
#define PERCOLATIONVERIFIER_H

#include <set>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;



set<unsigned> setUnion(set<unsigned> &set1, set<unsigned> &set2) {

    set<unsigned> unionSet;
    set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(unionSet, unionSet.end()));

    return unionSet;

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


TNode* lastResultParentWithSingleChild;

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
    tNode->leftBrother = NULL;
    tNode->rightBrother = NULL;
    tNode->parent = NULL;
    tNode->visited = false;

    return tNode;

}

TNode *lastParentCreated;

void buildCliqueTree(list<set<unsigned>> *cliquesList) {

    TNode *nav;
    lastParentCreated = NULL;

    // Cliques/leafs
    unsigned listSize = cliquesList->size();
    for (unsigned i = 0; i < listSize; i+=2) {


        TNode *newCliqueLeft = newTNode();

        newCliqueLeft->idClique = i;
        newCliqueLeft->leaf = true;
        newCliqueLeft->graphNodes = &cliquesList->front();

        cliquesList->pop_front();

        if (!cliquesList->empty()) {

            TNode *newCliqueRight = newTNode();
            newCliqueRight->idClique = i+1;
            newCliqueRight->leaf = true;

            newCliqueRight->graphNodes = &cliquesList->front();
            cliquesList->pop_front();

            newCliqueLeft->rightBrother = newCliqueRight;
            newCliqueRight->leftBrother = newCliqueLeft;

            TNode *newCliqueParent = newTNode();
            newCliqueParent->left = newCliqueLeft;
            newCliqueParent->right = newCliqueRight;

            set<unsigned> unionSet;
            set_union(newCliqueLeft->graphNodes->begin(), newCliqueLeft->graphNodes->end(), newCliqueRight->graphNodes->begin(), newCliqueRight->graphNodes->end(), inserter(unionSet, unionSet.end()));
            //set<unsigned> parentNodes = setUnion(*newCliqueLeft->graphNodes, *newCliqueRight->graphNodes);
            newCliqueParent->graphNodes = &unionSet;

            /*if (lastParentCreated != NULL) {
                lastParentCreated->rightBrother = newCliqueParent;
                newCliqueParent->leftBrother = lastParentCreated;
            }*/

            lastParentCreated = newCliqueParent;

        }

        if (i == 0) {
            nav = newCliqueLeft;

        }


    }

    //cout << lastParentCreated->graphNodes->size();

    /*while (nav != NULL ) {

        cout << nav->idClique << endl;

        if (nav != NULL) {
            nav = nav->rightBrother;
        }


    }*/


}

/*void buildCliqueTree(list<set<unsigned>> *cliquesList) {

    TNode *nav;

    // Cliques/leafs
    unsigned listSize = cliquesList->size();
    for (unsigned i = 0; i < listSize; i+=2) {


        TNode *newCliqueLeft = new TNode();

        newCliqueLeft->idClique = i;
        newCliqueLeft->leaf = true;
        newCliqueLeft->graphNodes = cliquesList->front();

        cliquesList->pop_front();

        if (!cliquesList->empty()) {

            TNode *newCliqueRight = new TNode();
            newCliqueRight->idClique = i+1;
            newCliqueRight->leaf = true;

            newCliqueRight->graphNodes = cliquesList->front();
            cliquesList->pop_front();

            newCliqueLeft->rightBrother = newCliqueRight;
            newCliqueRight->leftBrother = newCliqueLeft;
        }

        if (i == 0) {
            nav = newCliqueLeft;

        }


        TNode newCliqueParent;
        newCliqueParent.left = newCliqueLeft;
        newCliqueParent.right = newCliqueRight;

        newCliqueParent.graphNodes = setunion(newCliqueLeft, newCliqueRight);

    }



    while (nav->rightBrother != NULL ) {

        cout << nav->idClique << endl;

        if (nav->rightBrother != NULL) {
            nav = nav->rightBrother;
        }


    }


}*/




/*// Looks for an empty space to add a child on
// Basically it looks for a parent node with just one child node
// Returns the parent node itself
void recursivellyFindParentWithSigleChild(TNode &currentTreeNode) {

    if (lastResultParentWithSingleChild != NULL) {
        return;
    }

    // if only one of the childs is NULL
    if ((currentTreeNode.left == NULL) ^ (currentTreeNode.right == NULL)) {
        lastResultParentWithSingleChild = currentTreeNode;
        return;
    }

    if (currentTreeNode.left != NULL) {
        if (!currentTreeNode.left->leaf) {
            recursivellyFindParentWithSigleChild(currentTreeNode.left);
        }
    }

    if (currentTreeNode.right != NULL) {
        if (!currentTreeNode.right->leaf) {
            recursivellyFindParentWithSigleChild(currentTreeNode.right);
        }
    }
}

TNode* findParentWithSingleChild() {
    lastResultParentWithSingleChild = NULL;
    recursivellyFindParentWithSigleChild(TTree.root);
    return lastResultParentWithSingleChild;
}*/

#endif // PERCOLATIONVERIFIER_H
