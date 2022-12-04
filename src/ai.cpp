#include "ai.hpp"
#include "board.hpp"

AI::AI(Board& board)
    : m_board(board)
{
}

void AI::takeTurn(Unit& unit)
{
    std::vector<AStar::Path> paths;
    for (const auto& goal_unit : m_board.m_playerTeam)
    {
        // FIXME hacer que el tipo de mascara no tenga en cuenta
        // mis propios teammates, pero que si tome los teammates
        // enemigos como obstaculos
        paths.emplace_back(
                m_board.m_map.getPath(
                    unit.getPosition()
                    , goal_unit.getPosition()
                    , unit.getStats().awareness
                    , Terrain::Type::Castable));
    }

    auto it = std::find_if(paths.begin(), paths.end(),
            [](const AStar::Path& path)
            { return path.first == AStar::PathType::Valid; });
    if (it != paths.end())
    {
        // la distancia a recorrer va a ser la minima entre la capacidad
        // de movimiento de la unidad y la cantidad de distancia real
        // entre la unidad y la celda adyacente a su ojetivo mas cercana
        uint distance = std::min(static_cast<uint>(it->second.size() - 1)
                , unit.getStats().movement);

        /* DBG(distance); */

        /* DBG(unit.getPosition()); */
        /* DBG(it->second.at(1)); */

        m_board.moveCharacter(unit
                , it->second.at(it->second.size() - distance));
        m_board.m_turnManager.takeCtFromUnit(&unit, TurnManager::ActionTaken::Moved);
    }
}
