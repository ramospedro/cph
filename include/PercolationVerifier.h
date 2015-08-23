#ifndef PERCOLATIONVERIFIER_H
#define PERCOLATIONVERIFIER_H

#include <set>
#include <list>

using namespace std;

class PercolationVerifier
{
    public:
        PercolationVerifier();
        virtual ~PercolationVerifier();

        bool doTheseCliquesPercolate(set<unsigned> *clique1, set<unsigned> *clique2, unsigned numberOfNodesRequired) {

            bool percolate;

            set_intersection(origin, destine)
            set<unsigned> intersect;
            set_intersection(clique1.begin(),clique1.end(),clique2.begin(),clique2.end(), std::inserter(intersect,intersect.begin()));

            percolate = intersect.size() >= numberOfNodesRequired;

            return percolate;
        }


        list<unsigned> getPercolatingCliques(set<unsigned> *cliqueNodes) {
            recursivePercolationVerifier(cliqueNodes, cliqueTree.root);
            return lastResult;
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

        TTree cliqueTree;

        list<unsigned> lastResult;

        bool recursivePercolationVerifier(set<unsigned> *cliqueNodes, TNode *currentTreeNode) {

            if (currentTreeNode->left != NULL) {
                if (!currentTreeNode->left->visited) {
                    if (doTheseCliquesPercolate(*cliqueNodes, currentTreeNode->left->graphNodes, 1) {
                        // if it's a leaf node, then it's a percolating clique and needs to be added to the lastResult list
                        if (currentTreeNode->left.leaf) {
                            lastResult.add(currentTreeNode->left.idClique);
                        } else {
                        // if it's not a leaf, the recursion tree keeps going until it has found one
                            recursivePercolationVerifier(cliqueNodes, currentTreeNode->left);
                        }
                    }
                }
            }

            if (currentTreeNode->right != NULL) {
                if (!currentTreeNode->right.visited) {
                    if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode->right->graphNodes)) {
                        // if it's a leaf node, then it's a percolating clique and needs to be added to the lastResult list
                        if (currentTreeNode->right.leaf) {
                            lastResult.add(currentTreeNode->right.idClique);
                        } else {
                        // if it's not a leaf, the recursion tree keeps going until it has found one
                            recursivePercolationVerifier(cliqueNodes, currentTreeNode->right);
                        }
                    }
                }
            }

            currentTreeNode.visited = true;

            return true;
        }


    protected:
    private:

};

#endif // PERCOLATIONVERIFIER_H
