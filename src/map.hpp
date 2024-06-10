#pragma once

#include "img/img.hpp"
#include "itdReader.hpp"
#include <vector>
#include <array>

struct caseMap {
    int x;
    int y;
    std::array<float, 3> color;
    typeCase type;
    Node node = {};
};

struct Map {
    int height;
    int width;
    std::vector<caseMap> listCases;
};

Map checkImage(img::Image &baseMap);
Map compareMapItd(std::vector<Node> nodes, Map map);
void translateCoord(int x, int y, int &xOut, int &yOut, int sizex, int sizey);
std::vector<std::vector<float>> nodeToGraph(Map map);
std::unordered_map<int, std::pair<float, int>> dijkstra(std::vector<std::vector<float>> &graph, int const &start, int const end);