#include "itdReader.hpp"
#include "App.hpp"
#include "node.hpp"
#include "map.hpp"


#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "img/img.hpp"
#include <glm/glm.hpp>

#include <utility>
#include <iterator>

void displayEnum(typeCase type)
{
    switch (type)
    {
    case typeCase::path:
        std::cout << "path" << std::endl;
        break;
    case typeCase::in:
        std::cout << "in" << std::endl;
        break;
    case typeCase::out:
        std::cout << "out" << std::endl;
        break;
    case typeCase::none:
        std::cout << "none" << std::endl;
        break;
    default:
        std::cout << "Error" << std::endl;
        break;
    }
}

std::vector<std::vector<int>> checkMap()
{
    std::ifstream mapItd(make_absolute_path("data/.itd2", true));
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
        img::Image test{img::load(make_absolute_path("images/mapRGB-V2.png", true))};
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur fichier image" << std::endl;
        exit(1);
    }

    return nodes;
}

std::array<typeByColor, 3> getItdAllTypes()
{
    std::ifstream mapItd(make_absolute_path("data/.itd2", true));
    std::vector<std::string> lines;
    std::string line;
    std::array<typeByColor, 3> typesByColor;

    while (std::getline(mapItd, line))
    {
        if (line[0] != '#')
        {
            if (line.find("path") != std::string::npos || line.find("in") != std::string::npos || line.find("out") != std::string::npos)
            {
                typeByColor tbc;
                std::istringstream iss(line);
                std::string word;
                iss >> word;
                tbc.type = word == "path" ? typeCase::path : word == "in" ? typeCase::in
                                                                          : typeCase::out; // on récupère le type
                for (int i = 0; i < 3; i++)
                {
                    iss >> word;
                    tbc.color[i] = std::stof(word);
                }
                    typesByColor[tbc.type] = tbc;
            } 
        }
    }

    return typesByColor;
}

// Fonction pour comparer les noeuds + map et matcher les noeuds avec les cases correspondantes
Map compareMapItd(std::vector<Node> nodes, Map map)
{

    for (unsigned long i = 0; i < nodes.size(); i++)
    {
        Node node = nodes[i]; // on récupère le noeud
        if (node.x < 0 || node.y < 0 || node.x >= map.width || node.y >= map.height)
        {
            std::cerr << "Erreur coordonnées du noeud" << std::endl;
            exit(1);
        }
        caseMap currentCase = map.listCases[node.y * map.width + node.x];
        currentCase.node = node;
        map.listCases[node.y * map.width + node.x] = currentCase;
    }

    return map;
}

std::string getNameMap()
{
    std::ifstream mapItd(make_absolute_path("data/.itd2", true));
    std::string line;
    std::string name;

    while (std::getline(mapItd, line))
    {
        if (line.find("map") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string word;
            iss >> word;
            iss >> name;
            return name;
        }
    }

    return "";
}

