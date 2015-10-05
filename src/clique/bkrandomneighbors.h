#ifndef BKRANDOMNEIGHBORS_H
#define BKRANDOMNEIGHBORS_H

#include "cliquealgorithm.h"

class BKRandomNeighbors : public CliqueAlgorithm
{
    public:

        BKRandomNeighbors(LargeGraph *graph, int number);

        virtual void execute() override;
        virtual string getName() override;

        virtual float getAlpha() override;

    private:
        float alpha = 0;


        /// Heuristic method based on the Bron-Kerbosch method.
        /// It reorganizes the neighbors of a vertex into a random order, expecting to get to a clique in a faster way
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @return void
        void executeMethod(vector<unsigned> &P, set<unsigned> &R, vector<unsigned> &X);

        void setAlpha(float number);
};

#endif // BKRANDOMNEIGHBORS_H
