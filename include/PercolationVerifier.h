#ifndef PERCOLATIONVERIFIER_H
#define PERCOLATIONVERIFIER_H


class PercolationVerifier
{
    public:
        PercolationVerifier();
        virtual ~PercolationVerifier();

        unsigned getPercolatingClique(*set<unsigned> cliqueNodes) {
            return recursivePercolationVerifier(cliqueNodes, cliqueTree->root;)
        }

    protected:
    private:
        struct TNode {
            unsigned idClique;
            set<unsigned> graphNodes;
            TNode *left;
            TNode *right;
        };

        struct TTree {
            TNode *root;
        };

        TTree cliqueTree;

        unsigned recursivePercolationVerifier(*set<unsigned> cliqueNodes, TNode *currentTreeNode) {

            if (currentTreeNode->left == NULL and currentTreeNode->right == NULL) {
                return currentTreeNode->idClique;
            } else {

                if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode->left->graphNodes)) {
                    currentTreeNode = currentTreeNode->left;
                } else if (doTheseCliquesPercolate(cliqueNodes, currentTreeNode->right->graphNodes)) {
                    currentTreeNode = currentTreeNode->right;
                } else {
                    return -1;
                }

                return recursivePercolationVerifier(cliqueNodes, currentTreeNode);
            }
        }

        bool doTheseCliquesPercolate(*set<unsigned> clique1, *set<unsigned> clique2, unsigned numberOfNodesRequired) {

            bool percolate;

            set_intersection(origin, destine)
            set<unsigned> intersect;
            set_intersection(clique1.begin(),clique1.end(),clique2.begin(),clique2.end(), std::inserter(intersect,intersect.begin()));

            percolate = intersect.size() >= numberOfNodesRequired;

            return percolate;
        }
};

#endif // PERCOLATIONVERIFIER_H
