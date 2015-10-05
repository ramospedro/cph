#ifndef BRONKERBOSCHPIVOT_H
#define BRONKERBOSCHPIVOT_H

#include "cliquealgorithm.h"

class BronKerboschPivot: public CliqueAlgorithm
{
    public:

        BronKerboschPivot(LargeGraph *graph);

        virtual void execute() override;
        virtual string getName() override;

    private:

        /// Bron-Kerbosch method that makes use of a pivot vertex.
        /// It chooses a pivot vertex and tests to find out if it is part of the clique,
        /// if not, none of its neighbor are in that same clique
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @param chooseRandom if TRUE, chooses a pivot at random else, chooses the vertex with the most neighbors
        /// @return void
        void executeMethod(set<unsigned> &P, set<unsigned> &R, set<unsigned> &X);


        /// Method for choosing a pivot vertex, based on a random choice
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @return unsigned returns a random vertex in P
        unsigned randomPivot(set<unsigned> &P, set<unsigned> &X);


        /// Method for choosing a pivot vertex, based on the number of its neighbors
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @return unsigned returns the highest degree vertex
        unsigned wellKnownPivot(set<unsigned> &P, set<unsigned> &X);
};


#endif // BRONKERBOSCHPIVOT_H
