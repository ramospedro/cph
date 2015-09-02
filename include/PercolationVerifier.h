#ifndef PERCOLATIONVERIFIER_H
#define PERCOLATIONVERIFIER_H

#include <set>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;


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
    set<unsigned> graphNodes;
    TNode *left;
    TNode *right;
    bool visited;
    bool leaf;
};

struct TTree {
    TNode *root;
};

list<unsigned*> lastResult;


TNode* lastResultParentWithSingleChild;

bool recursivePercolationVerifier(set<unsigned> &cliqueNodes, TNode &currentTreeNode) {

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
}

// Looks for an empty space to add a child on
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
}

#endif // PERCOLATIONVERIFIER_H
