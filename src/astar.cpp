#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include "map.hpp"
#include "utils.hpp"
#include "astar.hpp"
#include <array>
#include <chrono>
#include <optional>

Node::Node(const sf::Vector2u& coordinates)
    : coords(coordinates)
{
}

Node::Node(uint x, uint y)
    : coords(x, y)
{
}

auto Node::getNeighbours(const Node& n) -> std::array<Node, 4>
{
    return {
        Node(n.coords.x    , n.coords.y - 1), // top
        Node(n.coords.x - 1, n.coords.y    ), // left
        Node(n.coords.x + 1, n.coords.y    ), // right
        Node(n.coords.x    , n.coords.y + 1), // bottom
    };
}

// pre-computo los máximos tamaños que va a tener el vector de closed y así
// solo reservo la memoria necesaria.
constexpr std::array<uint, 50> maxSizeByDistance = []()
{
    std::array<uint, 50> _dp;
    _dp[0] = 1;
    for (uint i = 1; i < _dp.size(); i++)
        _dp[i] = _dp[i-1] + i * 4;
    return _dp;
}();

AStar::AStar(const Map& map)
    : m_map(map)
{
}

auto AStar::getPath(
        sf::Vector2u const& start,
        sf::Vector2u const& goal,
        uint movement,
        Map::Terrain mask
        ) -> std::optional<std::vector<sf::Vector2u>>
{
    if (!m_map.isCoordInbounds(start) || !m_map.isCoordInbounds(goal)
            || !masksMatch(m_map.getTerrain(start), mask) || !masksMatch(m_map.getTerrain(goal), mask))
    {
        return std::nullopt;
    }

    m_open.clear();
    m_closed.clear();
    m_open.reserve(maxSizeByDistance.at(movement));
    m_closed.reserve(maxSizeByDistance.at(movement));
    m_open.emplace_back(start);
    while (!m_open.empty())
    {
        m_closed.emplace_back(getLowestFNode());
        m_open.pop_back();
        Node& current = m_closed.back();

        if (current.getFCost() > movement)
            continue;

        if (current.coords == goal)
        {
            std::vector<sf::Vector2u> path;
            auto it = &current;
            while (it != nullptr)
            {
                path.push_back(it->coords);
                it = it->parent;
            }
            return path;
        }

        auto neighbors = Node::getNeighbours(current);

        for (auto& neighbour : neighbors)
        {
            if (!m_map.isCoordInbounds(neighbour.coords)
                    || !masksMatch(m_map.getTerrain(neighbour.coords), mask)
                    || std::find(m_closed.begin(), m_closed.end(), neighbour) != m_closed.end())
            {
                continue;
            }

            auto distance = distanceVector(neighbour.coords, goal);
            neighbour.h_cost = distance.x + distance.y;
            neighbour.g_cost = current.g_cost + 1;

            if (auto it = std::find(m_open.begin(), m_open.end(), neighbour);
                    it != m_open.end())
            {
                if (it->getFCost() > neighbour.getFCost())
                {
                    it->h_cost = neighbour.h_cost;
                    it->g_cost = neighbour.g_cost;
                    it->parent = &current;
                }
            }
            else
            {
                neighbour.parent = &current;
                m_open.push_back(neighbour);
            }
        }
    }

    return std::nullopt;
}

Node& AStar::getLowestFNode()
{
    std::sort(m_open.begin(), m_open.end(), [](const Node& rhs, const Node& lhs)
            { if (rhs.getFCost() == lhs.getFCost()) return rhs.h_cost > lhs.h_cost;
            return rhs.getFCost() > lhs.getFCost(); });
    return m_open.back();
}

