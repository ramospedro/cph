#ifndef BRONKERBOSCH_H
#define BRONKERBOSCH_H

#include "cliquealgorithm.h"

class BronKerbosch : public CliqueAlgorithm
{
    public:

        BronKerbosch(LargeGraph *graph);

        virtual void execute() override;
        virtual string getName() override;

    private:

        /// Bron-Kerbosch method in its simplest form, tests every vertex for a possible clique
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @return void
        void executeMethod(set<unsigned> &P, set<unsigned>&R, set<unsigned>&X);
};

#endif // BRONKERBOSCH_H
