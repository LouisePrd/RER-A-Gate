#include "itdReader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "img/img.hpp"
#include <glm/glm.hpp>
#include "App.hpp"

std::vector<Node> getNodes(std::vector<std::vector<int>> nodes)
{
    std::vector<Node> nodesStruct;
    for (std::vector<int> node : nodes)
    {
        if (node.size() < 3 || node.size() > 4 || node[0] < 0 || node[1] < 0 || node[2] < 0)
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

std::vector<std::vector<int>> checkMap()
{
    std::ifstream mapItd(make_absolute_path("data/.itd", true));
    std::vector<std::string> keyWords = {"ITD", "map", "path", "in", "out", "graph", "node"};
    std::vector<std::vector<int>> nodes;
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(mapItd, line))
    {
        if (line[0] != '#')
        {
            lines.push_back(line);
        }
    }

    // On regarde le premier mot de chaque ligne
    for (std::string line : lines)
    {
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        if (word == "node")
        {
            std::vector<int> node;
            while (iss >> word)
            {
                node.push_back(std::stoi(word));
            }
            nodes.push_back(node);
        }
    }

    for (std::string keyWord : keyWords)
    {
        bool found = false;
        for (std::string line : lines)
        {
            if (line.find(keyWord) != std::string::npos)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cerr << "Erreur mot clé " << keyWord << " non trouvé" << std::endl;
            exit(1);
        }
    }

    // Vérifie fichier image
    try
    {
        img::Image test{img::load(make_absolute_path("images/level.png", true))};
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur fichier image" << std::endl;
        exit(1);
    }

    return nodes;

}

