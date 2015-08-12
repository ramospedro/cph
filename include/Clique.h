#ifndef CLIQUE_H
#define CLIQUE_H

class Clique
{
    public:
        Clique();
        virtual ~Clique();

        /*
        * The list of nodes composing the clique.
        */
        list<int> nodes;

        void AddNode(int node) {
            nodes.insert(node);
        }

    protected:
    private:
};

#endif // CLIQUE_H
