#include "bkcliqueremove.h"

BKCliqueRemove::BKCliqueRemove(LargeGraph *graph)
{
    this->graph = graph;
}



string BKCliqueRemove::getName()
{
    return "BKCliqueRemove";
}



void BKCliqueRemove::execute()
{
    vector<unsigned> P;
    set<unsigned> R;

    for (unsigned i = 0; i < this->graph->numberOfNodes; i++)
    {
        P.push_back(i);
    }

    random_shuffle (P.begin(), P.end());

    this->executeMethod(P, R);
}



void BKCliqueRemove::executeMethod(vector<unsigned> &P, set<unsigned> &R)
{
    // indicador do numero de chamadas ao método que são feitas
    chamadasRecursivas++;
    // indicador da profundidade da chamada atual
    profundidadeChamada++;

    // Se P for igual a zero, significa que não há mais vértices validos para agregar ao clique
    // R tem que ser maior que 1, ou seja, um clique só é válido com 2 ou mais vértices presentes
    if(P.size() == 0 && R.size() > 1)
    {
        cliqueEncontrado = true;
        this->cliques.push_back(R);
        return ;
    }
    else
    {
        // navegador aponta para o primeiro elemanto do conjunto P
        vector<unsigned>::iterator nav = P.begin();

        // percorre todos os elementos em P
        //
        while(nav != P.end())
        {
            // vertice recebe o valor do ponteiro de navegação
            unsigned v = *nav;

            // conjunto de vizinhos e novosP
            vector<unsigned> viz, nP;
            set<unsigned> vi;

            vi = getAdjacent(v);

            unsigned size = vi.size();

            for (unsigned i = 0; i < size; i++)
            {
                unsigned end = rand() % vi.size();
                set<unsigned>::iterator ini = vi.begin();
                for (unsigned j = 0; j<end;j++){
                    ini++;
                }
                unsigned valor =*ini;
                viz.push_back(valor);
                vi.erase(ini);
            }

            // configurando o comprimento de nR
            set<unsigned> nR(R.begin(), R.end());
            nR.insert(v);

            // nP = P ∩ viz
            nP = unsortedIntersectionBetween(P, viz);

            this->executeMethod(nP, nR);
            profundidadeChamada--;

            if (profundidadeChamada > 1)
            {
                return;
            }

            if (cliqueEncontrado)
            {
                cliqueEncontrado = false;

                list<set<unsigned>>::iterator currentClique = cliques.begin();
                set<unsigned>::iterator currentNode;

                int cliquePos = cliques.size() - 1;

                while(cliquePos > 0)
                {
                    currentClique++;
                    cliquePos--;
                }
                for(currentNode = currentClique->begin() ; currentNode != currentClique->end(); currentNode++)
                {
                    //remover;
                    P.erase(remove(P.begin(), P.end(), *currentNode),P.end());
                    //P.erase(currentNode);
                }
            }
            else
            {
                //remover;
                P.erase(remove(P.begin(), P.end(), v),P.end());
                //P.erase(v);
            }

            nav = P.begin();

        }
    }
}

