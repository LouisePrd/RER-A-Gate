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

Node getNodeById(std::vector<Node>& nodes, int id)
{
    for (Node& n : nodes)
    {
        if (n.id == id)
        {
            return n;
        }
    }
    std::cerr << "Erreur : noeud non trouvé" << std::endl;
    exit(1);
}

std::vector<Node> getNodes(std::vector<std::vector<int>>& nodes)
{
    std::vector<Node> allNodes;

    // Création des noeuds sans connections
    for (const std::vector<int>& node : nodes)
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

        allNodes.push_back(n);
    }

    // Ajout des connections
    for (int i = 0; i < nodes.size(); ++i)
    {
        for (int j = 3; j < nodes[i].size(); ++j)
        {
            int connectedId = nodes[i][j];
            allNodes[i].noeudsConnectes.push_back(connectedId);
            Node connectedNode = getNodeById(allNodes, connectedId);
            allNodes[i].noeudsConnectesStruct.push_back(connectedNode);
        }
    }

    return allNodes;
}