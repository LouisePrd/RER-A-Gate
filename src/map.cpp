#include "map.hpp"
#include "node.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "itdReader.hpp"
#include <queue>

Map checkImage(img::Image &baseMap)
{
    std::vector<caseMap> listCases;
    int width = baseMap.width();
    int height = baseMap.height();
    int size = width * height;

    for (int i = 0; i < size; i++)
    {
        int x = i % width;
        int y = height - 1 - (i / width);

        glm::vec3 color;
        color.r = baseMap.data()[i * 3];
        color.g = baseMap.data()[i * 3 + 1];
        color.b = baseMap.data()[i * 3 + 2];

        // On vérifie si la couleur correspond à un type de case
        std::array<float, 3> colorTab = {color.r, color.g, color.b};
        bool found = false;

        for (int j = 0; j < getItdAllTypes().size(); j++)
        {
            if (colorTab == getItdAllTypes()[j].color) // si la couleur correspond à un type de case
            {
                caseMap currentcaseMap{x, y, colorTab, getItdAllTypes()[j].type};
                listCases.push_back(currentcaseMap);
                found = true;
                break;
            }
        }

        if (!found) // si la couleur ne correspond à aucun type de case
        {
            caseMap currentcaseMap{x, y, colorTab, typeCase::none};
            listCases.push_back(currentcaseMap);
        }
    }

    Map map{height, width, listCases};
    return map;
}



// Graphes

void WeightedGraph::add_vertex(int const id)
{
    if (adjacency_list.find(id) == adjacency_list.end())
        adjacency_list[id] = std::vector<WeightedGraphEdge>();
}

void WeightedGraph::add_directed_edge(int const from, int const to, float const weight)
{
    add_vertex(from);
    add_vertex(to);
    WeightedGraphEdge edge{to, weight};
    adjacency_list[from].push_back(edge);
}

void WeightedGraph::add_undirected_edge(int const from, int const to, float const weight)
{
    add_directed_edge(from, to, weight);
    add_directed_edge(to, from, weight);
}

WeightedGraph build_from_adjacency_matrix(std::vector<std::vector<float>> const &adjacency_matrix)
{
    WeightedGraph graph;

    for (unsigned int i = 0; i < adjacency_matrix.size(); i++)
    {
        graph.add_vertex(i);
        for (unsigned int j = 0; j < adjacency_matrix[i].size(); j++)
        {
            if (adjacency_matrix[i][j] != 0)
            {
                graph.add_directed_edge(i, j, adjacency_matrix[i][j]);
            }
        }
    }
    return graph;
}

std::unordered_map<int, std::pair<float, int>> dijkstra(WeightedGraph const &graph, int const &start, int const end)
{
    std::unordered_map<int, std::pair<float, int>> distances;
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> to_visit;

    // Initialisation des distances
    for (const auto& node : graph.adjacency_list) {
        distances[node.first] = {std::numeric_limits<float>::infinity(), -1};
    }
    distances[start] = {0, -1};

    to_visit.push({0, start});

    while (!to_visit.empty())
    {
        auto [current_distance, current_node] = to_visit.top();
        to_visit.pop();

        // Si le nœud a déjà été traité avec une distance plus courte, on l'ignore
        if (current_distance > distances[current_node].first) {
            continue;
        }

        // Vérification si on a atteint le nœud de destination
        if (current_node == end) {
            break; // Arrêtez la boucle dès que nous atteignons le nœud de destination
        }

        for (WeightedGraphEdge edge : graph.adjacency_list.at(current_node))
        {
            float new_distance = current_distance + edge.weight;
            if (new_distance < distances[edge.to].first) {
                distances[edge.to] = {new_distance, current_node};
                to_visit.push({new_distance, edge.to});
            }
        }
    }

    return distances;
}

// création du graphe
std::vector<std::vector<float>> createGraph(Map map)
{
    std::vector<std::vector<float>> graph;
    int nbNodes = 0;

    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            caseMap currentCase = map.listCases[i * map.width + j];
            if (currentCase.node.noeudsConnectesStruct.size() !=0)
            {
                for (unsigned long k = 0; k < currentCase.node.noeudsConnectesStruct.size(); k++)
                {
                    nbNodes++;
                }
            }
        }
    }

    for (int i = 0; i < nbNodes; i++)
    {
        std::vector<float> row;
        for (int j = 0; j < nbNodes; j++)
        {
            row.push_back(0);
        }
        graph.push_back(row);
    }

    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            caseMap currentCase = map.listCases[i * map.width + j];
            if (currentCase.node.noeudsConnectesStruct.size() !=0)
            {
                for (unsigned long k = 0; k < currentCase.node.noeudsConnectesStruct.size(); k++)
                {
                    Node node = currentCase.node.noeudsConnectesStruct[k];
                    int distance = abs(currentCase.node.x - node.x) + abs(currentCase.node.y - node.y);
                    graph[currentCase.node.id][node.id] = distance;
                }
            }
        }
    }

    return graph;
}
