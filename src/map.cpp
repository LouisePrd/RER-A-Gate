#include "map.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "itdReader.hpp"
#include <queue>

Map checkImage(img::Image &baseMap)
{
    std::vector<caseMap> listCases;
    int size = baseMap.width() * baseMap.height();

    for (int i = 0; i < size; i++)
    {
        int x = i % baseMap.width();
        int y = i / baseMap.width();
        glm::vec3 color;
        color.r = baseMap.data()[i * 3];
        color.g = baseMap.data()[i * 3 + 1];
        color.b = baseMap.data()[i * 3 + 2];
        // on vérifie si la couleur correspond à un type de case
        std::array<float, 3> colorTab = {color.r, color.g, color.b};
        for (int j = 0; j < 3; j++)
        {
            if (colorTab == getItdAllTypes()[j].color) // si la couleur correspond à un type de case
            {
                caseMap currentcaseMap{x, y, colorTab, getItdAllTypes()[j].type};
                listCases.push_back({currentcaseMap});
                break;
            }
            else if (j == 2) // si la couleur ne correspond à aucun type de case
            {
                caseMap currentcaseMap{x, y, colorTab, typeCase::none};
                listCases.push_back({currentcaseMap});
                break;
            }
        }
    }

    // pour test affichage des types de cases
    for (unsigned long i = 0; i < listCases.size(); i++)
    {
        //caseMap currentCase = listCases[i];
        //displayEnum(currentCase.type);
    }

    Map map{static_cast<int>(baseMap.height()), static_cast<int>(baseMap.width()), listCases};
    return map;
}

Map compareMapItd(std::vector<Node> nodes, Map map)
{
    for (unsigned long i = 0; i < nodes.size(); i++)
    {
        Node node = nodes[i]; // on récupère le noeud
        for (unsigned long j = 0; j < map.listCases.size(); j++)
        {
            caseMap currentCase = map.listCases[j]; // on récupère la case
            if (node.x == currentCase.x && node.y == currentCase.y)
            {
                node.id = j;
            }
        }
    }
    return map;
}

std::vector<std::vector<float>> nodeToGraph(Map map)
{
    std::vector<std::vector<float>> graph(map.height, std::vector<float>(map.width, 0));

    for (int j = 0; j < map.height; j++)
    {
        for (int i = 0; i < map.width; i++)
        {
            int id = i + (map.height - 1 - j) * map.width;
            if (map.listCases[id].type == typeCase::path || map.listCases[id].type == typeCase::in || map.listCases[id].type == typeCase::out)
            {
                graph[j][i] = 1;
            }
            else
            {
                graph[j][i] = 0;
            }
        }
    }

    // on affiche la matrice
    for (int j = 0; j < map.height; j++)
    {
        for (int i = 0; i < map.width; i++)
        {
            std::cout << graph[j][i] << " ";
        }
        std::cout << std::endl;
    }

    return graph;
}


// Algo de Dijkstra

std::unordered_map<int, std::pair<float, int>> dijkstra(std::vector<std::vector<float>> &graph, int const &start, int const end)
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

        for (int i = 0; i < graph[current_node].size(); i++)
        {
            if (graph[current_node][i] == 0)
            {
                continue;
            }

            auto find_node{
                distances.find(i)
            };

            bool const visited{
                find_node != distances.end()
            };

            if (!visited)
            {
                int distance = current_distance + graph[current_node][i];
                distances[i] = {distance, current_node};
                to_visit.push({distance, i});
            }
            else
            {
                if (distances[i].first > current_distance + graph[current_node][i])
                {
                    distances[i] = {current_distance + graph[current_node][i], current_node};
                    to_visit.push({current_distance + graph[current_node][i], i});
                }
            }
        }
    }

    return distances;
}