#include "bronkerboschpivot.h"

BronKerboschPivot::BronKerboschPivot(LargeGraph *graph)
{
    this->graph = graph;
}



string BronKerboschPivot::getName()
{
    return "BronKerboschPivot";
}



void BronKerboschPivot::execute()
{
    set<unsigned> P, R, X;

    for (unsigned i = 0; i < this->graph->numberOfNodes; i++)
    {
        P.insert(i);
    }

    this->executeMethod(P, R, X);
}



void BronKerboschPivot::executeMethod(set<unsigned> &P, set<unsigned> &R, set<unsigned> &X)
{
    chamadasRecursivas++;
    if(P.size() == 0 && X.size() == 0)
    {
        this->cliques.push_back(R);
        return;
    }

    //============= P / N{u} ==================
    unsigned pivot;

    // escolhe aleatoriamento o pivo entre P U X
    //pivot = randomPivot(P, X);

    // escolhe o pivô com o maior número de vizinhos entre P U X
    pivot = wellKnownPivot(P, X);

    // pega todos os adjacentes do pivo
    set<unsigned> pivotAdj = getAdjacent(pivot);

    set<unsigned> possibleClique;

    std::set_difference(P.begin(), P.end(), pivotAdj.begin(), pivotAdj.end(),
                        std::inserter(possibleClique, possibleClique.end()));
    //==========================================

    set<unsigned>::iterator nav = P.begin();
    set<unsigned>::iterator vertexNav = possibleClique.begin();

    while(nav != possibleClique.end())
    {
        if(possibleClique.size() == 0)
        {
            return;
        }
        // *vertexNav faz parte do pivot, sem pivot o v recebe nav
        unsigned v = *vertexNav;

        // conjunto de vizinhos
        set<unsigned> viz = getAdjacent(v);

        // configurando o comprimento de nR
        set<unsigned> nR(R.begin(), R.end());
        nR.insert(v);

        // novosP e novosX
        set<unsigned> nP, nX;

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

        //==============Pivot====
        possibleClique.erase(v);
        vertexNav++;
        //=======================
    }
}



unsigned BronKerboschPivot::randomPivot(set<unsigned> &P, set<unsigned> &X)
{
    srand(time(NULL));

    set<unsigned> PX;
    set<unsigned>::iterator pxNav;

    std::set_union(P.begin(), P.end(), X.begin(), X.end(), std::inserter(PX, PX.end()));

    pxNav = PX.begin();
    int pivotSelector = rand() % PX.size();

    for(int i = 0; i < pivotSelector; i++)
    {
        pxNav++;
    }

    return *pxNav;
}



unsigned BronKerboschPivot::wellKnownPivot(set<unsigned> &P, set<unsigned> &X)
{
    //Metodo de seleção de pivo baseado na quantidade de vizinhos que o possivel pivo tem.
    set<unsigned> PX;
    set<unsigned>::iterator pxNav;

    std::set_union(P.begin(), P.end(), X.begin(), X.end(), std::inserter(PX, PX.end()));

    pxNav = PX.begin();
    int numberOfNeighbors = 0;
    unsigned newPivot;
    int range = PX.size();

    for(int i = 0; i < range; i++)
    {
        if(numberOfNeighbors < getAdjacent(*pxNav).size())
        {
            newPivot = *pxNav;
            numberOfNeighbors = getAdjacent(*pxNav).size();
        }
        pxNav++;
    }

    return newPivot;

}
