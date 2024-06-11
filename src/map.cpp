#include "map.hpp"
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


Map compareMapItd(std::vector<Node> nodes, Map map) {
    for (unsigned long i = 0; i < nodes.size(); i++) {
        Node node = nodes[i]; // on récupère le noeud
        if (node.x < 0 || node.y < 0 || node.x >= map.width || node.y >= map.height) {
            std::cerr << "Erreur coordonnées du noeud" << std::endl;
            exit(1);
        }

        for (unsigned long j = 0; j < map.listCases.size(); j++) { // on parcourt les cases de la map
            caseMap currentCase = map.listCases[j];
            if (currentCase.x == node.x && currentCase.y == node.y) { 
                currentCase.node = node;
                map.listCases[j] = currentCase;
                std::cout << "node en (" << node.x << ";" << node.y << ")";
                std::cout << " correspond à la case (" << currentCase.x << ";" << currentCase.y << ")";
                std::cout << " et contient les noeuds connectés : ";
                for (unsigned long k = 0; k < node.noeudsConnectes.size(); k++) {
                    std::cout << node.noeudsConnectes[k] << " ";
                }
                std::cout << std::endl;
            }
            
        }

    }
    return map;
}

std::vector<std::vector<float>> nodeToGraph(Map map)
{
    std::vector<std::vector<float>> graph(map.height, std::vector<float>(map.width, 0));

    return graph;
}

// Graphes
void WeightedGraph::add_vertex(int const id)
{
    if (adjacency_list.find(id) == adjacency_list.end())
        adjacency_list[id] = std::vector<WeightedGraphEdge>();
}

void WeightedGraph::add_directed_edge(int const from, int const to, float const weight)
{
    // Vérifie que les sommets existent, sinon on les crée
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
    std::unordered_map<int, std::pair<float, int>> distances{};
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> to_visit{};

    to_visit.push({0, start});
    while (!to_visit.empty())
    {
        auto [current_distance, current_node]{to_visit.top()};
        to_visit.pop();

        if (current_node == end)
        {
            return distances;
        }

        for (WeightedGraphEdge edge : graph.adjacency_list.at(current_node))
        {
            auto find_node{
                distances.find(edge.to)
            };

            bool const visited{
                find_node != distances.end()
            };

            if (!visited)
            {
                int distance = current_distance + edge.weight;
                distances[edge.to] = {distance, current_node};
                to_visit.push({distance, edge.to});

            }
            else
            {
                if (distances[edge.to].first > current_distance + edge.weight)
                {
                    distances[edge.to] = {current_distance + edge.weight, current_node};
                    to_visit.push({current_distance + edge.weight, edge.to});
                }
            }
        }
    }

    return distances;
}