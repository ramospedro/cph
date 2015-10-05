#include "bronkerbosch.h"

BronKerbosch::BronKerbosch(LargeGraph *graph)
{
    this->graph = graph;
}


string BronKerbosch::getName()
{
    return "BronKerbosch";
}



void BronKerbosch::execute()
{
    set<unsigned> P, R, X;

    for (unsigned i = 0; i < this->graph->numberOfNodes; i++)
    {
        P.insert(i);
    }

    this->executeMethod(P, R, X);
}



void BronKerbosch::executeMethod(set<unsigned> &P,set<unsigned>&R,set<unsigned>&X)
{
    chamadasRecursivas++;
    if(P.size()==0 && X.size()==0)
    {
        this->cliques.push_back(R);
        return ;
    }
    else
    {
        // navegador aponta para o primeiro elemanto do conjunto P
        set<unsigned>::iterator nav = P.begin();

        // percorre todos os elementos em P
        while(nav != P.end())
        {

            // vertice recebe o valor do ponteiro de navegação
            unsigned v=*nav;

            // conjunto de vizinhos e novosP
            set<unsigned> viz, nP;

            // seleciona o metodo para escolha dos vizinhos
            viz = getAdjacent(v);

            // configurando o comprimento de nR
            set<unsigned> nR(R.begin(), R.end());
            nR.insert(v);
            set<unsigned> nX;

            // nP = P ∩ viz
            std::set_intersection(viz.begin(), viz.end(),
                                  P.begin(), P.end(),
                                  std::inserter(nP, nP.end()));
            // nX = X ∩ viz
            std::set_intersection(viz.begin(), viz.end(),
                                  X.begin(), X.end(),
                                  std::inserter(nX, nX.end()));

            this->executeMethod(nP, nR, nX);

            //remover;
            P.erase(v);
            //incluir;
            X.insert(v);
            nav = P.begin();
        }
    }
}


