#include "cliquealgorithm.h"


CliqueAlgorithm::CliqueAlgorithm()
{

}

set<unsigned> CliqueAlgorithm::getAdjacent(unsigned vertex)
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




vector<unsigned> CliqueAlgorithm::unsortedIntersectionBetween(vector<unsigned> &collection, vector<unsigned> &neighbors)
{
    vector<unsigned>::iterator itCollection = collection.begin();
    vector<unsigned>::iterator itNeighbors = neighbors.begin();

    vector<unsigned> intersection;

    while(itCollection != collection.end())
    {
        while(itNeighbors != neighbors.end())
        {
            if (*itNeighbors == *itCollection)
            {
                intersection.push_back(*itNeighbors);
            }
            itNeighbors++;
        }
        itNeighbors = neighbors.begin();
        itCollection++;
    }
    return intersection;
}




float CliqueAlgorithm::getAlpha()
{
    return 1.0;
}


list<set<unsigned>> CliqueAlgorithm::getCliques()
{
    return this->cliques;
}

