#include "itdReader.hpp"
#include "App.hpp"
#include "node.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "img/img.hpp"
#include <glm/glm.hpp>

#include <utility>
#include <iterator>

std::vector<Node> getNodes(std::vector<std::vector<int>> nodes)
{
    std::vector<Node> nodesStruct;
    std::vector<Node> noeudsListe;

    for (std::vector<int> node : nodes)
    {
        if (node.size() < 3 || node[0] < 0 || node[1] < 0 || node[2] < 0)
        {
            std::cerr << "Erreur nombre d'arguments pour un noeud" << std::endl;
            exit(1);
        }
        Node n;
        n.id = node[0];
        n.x = node[1];
        n.y = node[2];
        for (unsigned long i = 3; i < node.size(); i++)
        {
            n.noeudsConnectes.push_back(node[i]);
        }

        nodesStruct.push_back(n);  

    }

    return nodesStruct;
}


void getNodeById(std::vector<Node> nodes, int id, Node &node)
{
    for (Node n : nodes)
    {
        if (n.id == id)
        {
            node = n;
            return;
        }
    }
    std::cerr << "Erreur noeud non trouvé" << std::endl;
    exit(1);
    
}