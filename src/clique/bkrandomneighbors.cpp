#include "bkrandomneighbors.h"

BKRandomNeighbors::BKRandomNeighbors(LargeGraph *graph, int number)
{
    this->graph = graph;
    this->setAlpha(number);
}



string BKRandomNeighbors::getName()
{
    return "BKRandomNeighbors";
}



float BKRandomNeighbors::getAlpha()
{
    return this->alpha;
}



void BKRandomNeighbors::execute()
{
    set<unsigned> R;

    vector<unsigned> P;
    vector<unsigned> X;

    for (unsigned i = 0; i < this->graph->numberOfNodes; i++)
    {
        P.push_back(i);
    }

    this->executeMethod(P, R, X);
}



void BKRandomNeighbors::executeMethod(vector<unsigned> &P, set<unsigned> &R, vector<unsigned> &X)
{
    chamadasRecursivas++;
    if(P.size() == 0 && X.size() == 0 && R.size() > 1)
    {
        this->cliques.push_back(R);
        return ;
    }
    else
    {
        // navegador aponta para o primeiro elemanto do conjunto P
        vector<unsigned>::iterator nav = P.begin();

        // percorre todos os elementos em P
        while(nav != P.end())
        {

            // vertice recebe o valor do ponteiro de navegação
            unsigned v=*nav;

            // conjunto de vizinhos
            set<unsigned> vi;


//======================SHUFFLE NEIGHBORS AT RANDOM================================
            vi = getAdjacent(v);
            vector<unsigned> viz;
            unsigned size = vi.size();
            for (unsigned i=0;i<size*(this->alpha);i++){
                unsigned end = rand() % vi.size();
                set<unsigned>::iterator ini = vi.begin();
                for (unsigned j = 0; j<end;j++){
                    ini++;
                }
                unsigned valor =*ini;
                viz.push_back(valor);
                vi.erase(ini);
            }
//=================================================================================

            // configurando o comprimento de nR
            set<unsigned> nR(R.begin(), R.end());
            nR.insert(v);

            vector<unsigned> nX;
            vector<unsigned> nP;

            // nP = P ∩ viz
            nP = unsortedIntersectionBetween(P, viz);

            // nX = X ∩ viz
            nX = unsortedIntersectionBetween(X, viz);

            this->executeMethod(nP, nR, nX);

            //remover;
            P.erase(nav);
            //incluir;
            X.push_back(v);
            nav = P.begin();

        }
    }
}



void BKRandomNeighbors::setAlpha(float number = 1)
{
    number = number / 100.0;

    if(number >= 1)
    {
        this->alpha = 1;
    }
    else if (number <= 0)
    {
        this->alpha = 0;
    }
    else
    {
        this->alpha = number;
    }
}


