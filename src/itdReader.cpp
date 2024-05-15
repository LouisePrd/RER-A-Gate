#include "itdReader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "utils.hpp"
#include "img/img.hpp"

void checkMap()
{
    std::ifstream mapItd(make_absolute_path("data/.itd", true));
    std::vector<std::string> keyWords = {"ITD", "map", "path", "in", "out", "graph", "node"};
    std::vector<std::vector<int>> nodes;
    std::vector<std::string> lines;

    std::string line;
    std::string lineNodes;
    int nbLinesTotal = 0;
    int nbLinesNodes = 0;

    while (std::getline(mapItd, line)) // Sans les commentaires
    {
        if (line[0] != '#')
        {
            lines.push_back(line);
        }
        nbLinesTotal++;
    }

    std::vector<std::string> values;
    std::string delimiter = " ";
    bool findKeyWords = false;
    for (size_t i = 0; i < lines.size(); i++)
    {
        size_t pos = 0;
        std::string token;
        while ((pos = lines[i].find(delimiter)) != std::string::npos)
        {
            token = lines[i].substr(0, pos);
            values.push_back(token);
            lines[i].erase(0, pos + delimiter.length());
        }
        values.push_back(lines[i]);
    }

    for (size_t i = 0; i < values.size(); i++)
    {
        std::cout << values[i] << ' ';
        
    }

    size_t nbValues = values.size();
    for (size_t i = 0; i < nbValues; i++) // Vérifie valeurs RGB
    {
        if (nbValues != 9 || std::stoi(values[i]) < 0 || std::stoi(values[i]) > 255)
        {
            std::cerr << "Erreur fichier pour : " << keyWords[i] << std::endl;
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

    exit(0);
}