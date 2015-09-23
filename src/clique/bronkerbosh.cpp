#include "bronkerbosh.h"

BronKerbosch::BronKerbosch(LargeGraph * graph)
{
    this->graph = graph;

    srand(time(NULL));
    //srand(24);
}



void BronKerbosch::execute()
{
    set<unsigned> P;

    for (unsigned i=0; i<this->graph->numberOfNodes;i++)
    {
        P.insert(i);
    }

    set<unsigned> R, X;

    // Bron-kerbosch simples => 100% dos cliques e muitas chamadas recursivas
    //this->executeRec(P, R, X, false);

    //Bron-Kerbosh pivô => 100% dos cliques e pucas chamadas recursivas
    this->executePivot(P, R, X, false);

    // Bron-Kerbosh pivô => 100% dos cliques e pucas/muitas chamadas recursivas
    //this->executePivot(P, R, X, true);

    //=======[UNDER DEVELOPMENT]=======
    // Bron-Kerbosh pivô => diminue o numero de cliques encontrados e de chamadas recursivas
    // (aumenta o tempo, não comfirmado ainda)
    //this->setAlfa(100); // alcance de 100% a 1%, não representa o numero de cliques
    //this->executeRec(P, R, X, true);

    //=======[UNDER DEVELOPMENT]=======
    // utiliza o alfa para saber qunatos adjacentes ele mistura
    //this->setAlfa(100);
    //this->executeRecRandom(P, R, X);

    // Heurística baseada em Bron-Kerbosh que remove todos os vertices do clique de P
    // Cada vértice pertence a um clique nesse método
    // Leva cerca de 1 minuto para percorrer o grafo gowalla_edges_un.net, com cerca de 190 mil vertices
    // this->executeRemoveCliqueFound(P, R);
}



void BronKerbosch::executeRec(set<unsigned> &P,set<unsigned>&R,set<unsigned>&X, bool useAlpha)
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
            if (useAlpha)
            {
                viz = alfaGetAdjacent(v, X);
            }
            else
            {
                viz = getAdjacent(v);
            }

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

            this->executeRec(nP, nR, nX, useAlpha);

            //remover;
            P.erase(v);
            //incluir;
            X.insert(v);
            nav = P.begin();

        }
    }
}



void BronKerbosch::executeRecRandom(set<unsigned> &P,set<unsigned>&R,set<unsigned>&X)
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
            set<unsigned> vi, nP;


//======================SHUFFLE NEIGHBORS AT RANDOM================================
            vi = getAdjacent(v);
            list<unsigned> viz;
            unsigned size = vi.size();
            for (unsigned i=0;i<size*(this->alfa);i++){
                unsigned end = rand() % vi.size();
                set<unsigned>::iterator ini = vi.begin();
                for (unsigned j = 0; j<end;j++){
                    ini++;
                }
                unsigned valor =*ini;
                viz.push_front(valor);
                vi.erase(ini);
            }
//=================================================================================

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

            this->executeRecRandom(nP, nR, nX);

            //remover;
            P.erase(v);
            //incluir;
            X.insert(v);
            nav = P.begin();

        }
    }
}



void BronKerbosch::executeRemoveCliqueFound(set<unsigned> &P, set<unsigned> &R)
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
        set<unsigned>::iterator nav = P.begin();

        // percorre todos os elementos em P
        //
        while(nav != P.end())
        {
            // vertice recebe o valor do ponteiro de navegação
            unsigned v=*nav;

            // conjunto de vizinhos e novosP
            set<unsigned> viz, nP;

            viz = getAdjacent(v);

            // configurando o comprimento de nR
            set<unsigned> nR(R.begin(), R.end());
            nR.insert(v);

            // nP = P ∩ viz
            std::set_intersection(viz.begin(), viz.end(),
                                  P.begin(), P.end(),
                                  std::inserter(nP, nP.end()));

            //=====LOG====
//            logSetCollection(P,"P");
//            logSetCollection(R,"R");
//            logUnsigned(v,"Vertice");
//            logSetCollection(viz,"Vizinhos");
//            logSetCollection(nP,"nP");
//            logSetCollection(nR,"nR");
//            logInt(chamadasRecursivas,"Nº Chamadas");
//            logInt(profundidadeChamada,"Profundidade");
//            logBool(cliqueEncontrado,"Encontrado");
//            logInt(cliques.size(), "N° cliques");
//            logString("[ANTES CHAMADA]", to_string(chamadasRecursivas));
//            logString("","");
//            logString("","");
//            //============

            this->executeRemoveCliqueFound(nP, nR);
            profundidadeChamada--;

            //=====LOG====
//            logSetCollection(P,"P");
//            logSetCollection(R,"R");
//            logUnsigned(v,"Vertice");
//            logSetCollection(viz,"Vizinhos");
//            logSetCollection(nP,"nP");
//            logSetCollection(nR,"nR");
//            logInt(chamadasRecursivas,"Nº Chamadas");
//            logInt(profundidadeChamada,"Profundidade");
//            logBool(cliqueEncontrado,"Encontrado");
//            logString("[DEPOIS CHAMADA]", to_string(chamadasRecursivas));
//            logString("","");
//            logString("","");
            //============

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
                    P.erase(*currentNode);
                }
            }
            else
            {
                //remover;
                P.erase(v);
            }

            nav = P.begin();

        }
    }
}



void BronKerbosch::executePivot(set<unsigned> &P, set<unsigned> &R, set<unsigned> &X, bool chooseRandom)
{
    chamadasRecursivas++;
    if(P.size() == 0 && X.size() == 0)
    {
        this->cliques.push_back(R);
        return;
    }

    //============= P / N{u} ==================
    unsigned pivot;

    if(chooseRandom)
    {
        // escolhe aleatoriamento o pivo entre P U X
        pivot = randomPivot(P, X);
    }
    else
    {
        // escolhe o pivô com o maior número de vizinhos entre P U X
        pivot = wellKnownPivot(P, X);
    }

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

        this->executePivot(nP, nR, nX, chooseRandom);

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



void BronKerbosch::printCliques(list<set<unsigned> > cliques)
{
    list<set<unsigned>>::iterator currentClique = cliques.begin();
    set<unsigned>::iterator currentNode;

    for( ; currentClique != cliques.end(); currentClique++)
    {

        for(currentNode = currentClique->begin() ; currentNode != currentClique->end(); currentNode++)
        {
            cout << *currentNode << " ";
        }
        cout << endl;
    }
}



unsigned BronKerbosch::randomPivot(set<unsigned> &P, set<unsigned> &X)
{
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



unsigned BronKerbosch::wellKnownPivot(set<unsigned> &P, set<unsigned> &X)
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



set<unsigned> BronKerbosch::getAdjacent(unsigned vertex)
{
    set<unsigned> vertexAdj;

    unordered_map<unsigned, LargeGraphEdge >::iterator adjNav = this->graph->adjNodes[vertex].begin();

    while(adjNav != this->graph->adjNodes[vertex].end())
    {
        if(adjNav->first != vertex)
        {
            vertexAdj.insert(adjNav->first);
        }
            adjNav++;
    }
    return vertexAdj;
}



set<unsigned> BronKerbosch::alfaGetAdjacent(unsigned vertex, set<unsigned> &X)
{

    set<unsigned> vertexAdj = getAdjacent(vertex);

    set<unsigned>::iterator iteratorAdj = vertexAdj.begin();

    set<unsigned> alfaAdj;

    int totalVerticesToRemove = vertexAdj.size() - ceil(vertexAdj.size() * alfa);
    if (totalVerticesToRemove == 0)
    {
        return vertexAdj;
    }

    int verticesToRemove[totalVerticesToRemove];
    int remove;

    for(int i = 0; i < totalVerticesToRemove; i++)
    {
        remove = rand() % vertexAdj.size();
        if(i == 0)
        {
            verticesToRemove[i] = remove;
            X.insert(remove);
        }
        //roda a lista para remover e compara se foi sorteado novamente
        for(int j = 0; j < i; j++)
        {
            // ja foi sorteado
            if(verticesToRemove[j] == remove)
            {
                i--;
                break;
            }
            //nao foi sorteado, entao insere
            verticesToRemove[i] = remove;
            X.insert(remove);
        }
    }

    for(int i = 0; iteratorAdj != vertexAdj.end(); iteratorAdj++)
    {
        if(*iteratorAdj != verticesToRemove[i])
        {
            alfaAdj.insert(*iteratorAdj);
            i++;
        }
    }

    return alfaAdj;

}



void BronKerbosch::setAlfa(float number = 1)
{
    number = number / 100.0;

    if(number >= 1)
    {
        this->alfa = 1;
    }
    else if (number <= 0)
    {
        this->alfa = 0;
    }
    else
    {
        this->alfa = number;
    }
}


//LOG CLASS

void BronKerbosch::logSetCollection(set<unsigned> &collection, string  label)
{
    string message = "";
    set<unsigned>::iterator debugP = collection.begin();
    while(debugP != collection.end())
    {
        message = message + to_string(*debugP) + " ";
        debugP++;
    }

    writeLog(message, label);
}

void BronKerbosch::logBool(bool boolean, string  label)
{
    writeLog(boolean ? "TRUE" : "FALSE", label);
}

void BronKerbosch::logInt(int value, string label)
{
    string message;
    message = to_string(value);

    writeLog(message, label);
}

void BronKerbosch::logUnsigned(unsigned value, string label)
{
    string message;
    message = to_string(value);

    writeLog(message, label);
}

void BronKerbosch::logString(string  message, string  label)
{
    writeLog(message, label);
}

void BronKerbosch::writeLog(string message, string label)
{
    ofstream log(fileName, ios_base::app | ios_base::out);
    log << label << ": " << message << endl;
    log.close();
}
