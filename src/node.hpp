#pragma once
#include "itdReader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "img/img.hpp"
#include <glm/glm.hpp>

#include <utility>
#include <iterator>


struct Node
{
    int id;
    int x;
    int y;
    std::vector<int> noeudsConnectes;
    std::vector<Node> noeudsConnectesStruct;
};

Node getNodeById(std::vector<Node>& nodes, int id);
std::vector<Node> getNodes(std::vector<std::vector<int>>& nodes);

