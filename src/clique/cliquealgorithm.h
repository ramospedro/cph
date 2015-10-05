#ifndef CLIQUEALGORITHM_H
#define CLIQUEALGORITHM_H

#include "../graph/largegraph.h"
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdlib.h> //rand()
#include <typeinfo>

using namespace std;

//STRATEGY
class CliqueAlgorithm
{
    public:

        int chamadasRecursivas = 0;

        LargeGraph *graph;
        list<set<unsigned>> cliques;

        virtual void execute() = 0;
        virtual string getName() = 0;
        virtual float getAlpha();


        /// Method for getting all the adjecent vertecies from a vertex
        /// @param vertex the central vertex
        /// @return set<unsigned> returns a set of all the adjecent vertices
        set<unsigned> getAdjacent(unsigned vertex);

        vector<unsigned> unsortedIntersectionBetween(vector<unsigned> &collection, vector<unsigned> &neighbors);

        list<set<unsigned>> getCliques();

        CliqueAlgorithm();
};


#endif // CLIQUEALGORITHM_H
