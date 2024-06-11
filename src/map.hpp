#pragma once

#include "img/img.hpp"
#include "itdReader.hpp"
#include <vector>
#include <array>

struct caseMap
{
    int x;
    int y;
    std::array<float, 3> color;
    typeCase type;
    Node node = {};
};

struct Map
{
    int height;
    int width;
    std::vector<caseMap> listCases;
};

struct WeightedGraphEdge
{
    int to{};
    float weight{1.0f};
};

struct WeightedGraph
{
    std::unordered_map<int, std::vector<WeightedGraphEdge>> adjacency_list{};

    void add_vertex(int const id);

    void add_directed_edge(int const from, int const to, float const weight = 1.0f);
    void add_undirected_edge(int const from, int const to, float const weight = 1.0f);

    bool operator==(WeightedGraph const &other) const;
    bool operator!=(WeightedGraph const &other) const;
};

Map checkImage(img::Image &baseMap);
Map compareMapItd(std::vector<Node> nodes, Map map);
void translateCoord(int x, int y, int &xOut, int &yOut, int sizex, int sizey);
std::vector<std::vector<float>> nodeToGraph(Map map);

// Graphes
WeightedGraph build_from_adjacency_matrix(std::vector<std::vector<float>> const &adjacency_matrix);
std::unordered_map<int, std::pair<float, int>> dijkstra(WeightedGraph const &graph, int const &start, int const end);