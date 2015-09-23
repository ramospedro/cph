#ifndef BRONKERBOSH_H
#define BRONKERBOSH_H


#include "../graph/largegraph.h"
#include <set>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h> //usleep()
#include <stdlib.h> //rand()
#include <math.h>  //celi()

//Log class
#include <fstream>
#include <ios>


using namespace std;


class BronKerbosch
{
    public:
        int chamadasRecursivas = 0;
        int profundidadeChamada = 0;
        bool cliqueEncontrado  = false;

        LargeGraph *graph;
        list< set<unsigned> > cliques;


        BronKerbosch(LargeGraph *graph);

        void execute();

        /// Bron-Kerbosch method in its simplest form, tests every vertex for a possible clique
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @param useAlpha set as TRUE to use alpha to delimit the number of neighbors af a vertex
        /// @return void
        void executeRec(set<unsigned> &P, set<unsigned>&R, set<unsigned>&X, bool useAlpha);

        /// Bron-Kerbosch method that makes use of a pivot vertex.
        /// It chooses a pivot vertex and tests to find out if it is part of the clique,
        /// if not, none of its neighbor are in that same clique
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @param chooseRandom if TRUE, chooses a pivot at random else, chooses the vertex with the most neighbors
        /// @return void
        void executePivot(set<unsigned> &P, set<unsigned> &R, set<unsigned> &X, bool chooseRandom);

        /// Heuristic method based on the Bron-Kerbosch method.
        /// It reorganizes the neighbors of a vertex into a random order, expecting to get to a clique in a faster way
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        /// @param X an set of type unsigned with all vertices that already have been tested
        /// @return void
        void executeRecRandom(set<unsigned> &P,set<unsigned>&R,set<unsigned>&X);


        /// Heuristic method based on the Bron-Kerbosch method.
        /// After finding a clique, the heuristic removes all the vertices in the new found clique from P
        /// In this heuristic, a vertex is present in only one clique, meaning that there is no overlap
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        void executeRemoveCliqueFound(set<unsigned> &P, set<unsigned> &R);


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

        /// Method for getting all the adjecent vertecies from a vertex
        /// @param vertex the central vertex
        /// @return set<unsigned> returns a set of all the adjecent vertices
        set<unsigned> getAdjacent(unsigned vertex);

        set<unsigned>   alfaGetAdjacent(unsigned vertex, set<unsigned> &X);

        void setAlfa(float number);

        void printCliques(list<set<unsigned> > cliques);




        //Log class
        void logSetCollection(set<unsigned> &collection, string  label);
        void logBool(bool boolean, string  label);
        void logInt(int value, string  label);
        void logUnsigned(unsigned value, string  label);
        void logString(string  message, string  label);
        void writeLog(string  message, string  label);

    private:

        float alfa = 0;

        //Log class
        string fileName = "log_file.txt";

};

#endif // BRONKERBOSH_H
