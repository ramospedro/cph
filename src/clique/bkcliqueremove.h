#ifndef BKCLIQUEREMOVE_H
#define BKCLIQUEREMOVE_H

#include "cliquealgorithm.h"

// Heurística baseada em Bron-Kerbosch que remove todos os vertices do clique de P
// Cada vértice pertence a um clique nesse método
// Leva cerca de 1 minuto para percorrer o grafo gowalla_edges_un.net, com cerca de 190 mil vertices

using namespace std;

class BKCliqueRemove : public CliqueAlgorithm
{
    public:
        int profundidadeChamada = 0;
        bool cliqueEncontrado = false;

        BKCliqueRemove(LargeGraph *graph);

        virtual void execute() override;
        virtual string getName() override;

    private:

        /// Heuristic method based on the Bron-Kerbosch method.
        /// After finding a clique, the heuristic removes all the vertices in the new found clique from P
        /// In this heuristic, a vertex is present in only one clique, meaning that there is no overlap
        /// @param P an set of type unsigned with all vertices of a graph
        /// @param R an set of type unsigned with all vertices that compose the current clique
        void executeMethod(vector<unsigned> &P, set<unsigned> &R);

};

#endif // BKCLIQUEREMOVE_H
