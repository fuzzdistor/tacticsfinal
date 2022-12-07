#include "ai.hpp"
#include "SFML/System/Vector2.hpp"
#include "board.hpp"
#include "terrain.hpp"
#include "turnmanager.hpp"
#include "utils.hpp"

AI::AI(Board& board)
    : m_board(board)
{
}

void AI::takeTurn(Unit& unit)
{
    std::vector<std::vector<sf::Vector2u>> paths;

    // miro en todas las unidades y elijo la que esté más cerca y accesible
    for (const auto& goal_unit : m_board.m_units)
    {
        if (goal_unit.getFaction() != unit.getFaction())
        {
            if (auto distanceV = distanceVector(unit.getCoordinates(), goal_unit.getCoordinates());
                    distanceV.x + distanceV.y >= unit.getStats().awareness)
                continue;

            auto neighbourNodes = AStarNode::getNeighbours(AStarNode(goal_unit.getCoordinates()));
            for (auto& neighbour : neighbourNodes)
            {
                auto path = m_board.m_map.getPath(
                        unit.getCoordinates()
                        , neighbour.coords
                        , unit.getStats().awareness
                        , Terrain::Type::Castable);
                if (path.first == AStar::PathType::Valid)
                paths.push_back(std::move(path.second));
            }
        }
    }

    D(unit.getName() << " attempts to move");
    if (!paths.empty())
    {
        std::sort(paths.begin(), paths.end(), [](const auto& lhs, const auto& rhs)
                { return lhs.size() < rhs.size(); });

        auto& path = paths.front();

        assert(path.size() >= 1 && "In AI: The target position and unit position are overlapping");
        // la distancia a recorrer va a ser la minima entre la capacidad
        // de movimiento de la unidad y la cantidad de distancia real
        // entre la unidad y la celda adyacente a su ojetivo mas cercana
        //
        uint distance = std::min(static_cast<uint>(path.size())
                , unit.getStats().movement + 1);

        D("Attempting to move to: " << path.at(path.size() - distance));
        D("Contents of path:");
        for (auto& position : path)
            D("position: " << position);
        m_board.moveCharacter(unit , path.at(path.size() - distance));
        if (m_board.m_combatManager.tryAttack(unit))
            m_board.advanceTurn(TurnManager::ActionTaken::MovedAndAction);
        else
            m_board.advanceTurn(TurnManager::ActionTaken::Moved);
    }
    else
    {
        D(unit.getName() << " did not find a unit to approach");
        D(unit.getName() << " stays in place");
        m_board.advanceTurn(TurnManager::ActionTaken::None);
    }
}
