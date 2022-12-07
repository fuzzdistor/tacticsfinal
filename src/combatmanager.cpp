#include "combatmanager.hpp"
#include "astar.hpp"
#include <algorithm>

CombatManager::CombatManager(const Map& map, std::vector<Unit>& units)
    : m_map(map)
    , m_units(units)
    , rng()
{
}

std::vector<Unit*> CombatManager::getPosibleTargets(const Unit &attacker)
{
    std::vector<Unit*> targets;
    auto neighbours = AStarNode::getNeighbours(AStarNode(attacker.getCoordinates()));
    for (auto& neighbour : neighbours)
    {
        if (!m_map.isCoordInbounds(neighbour.coords))
            continue;

        if (m_map.getTerrain(neighbour.coords) == Terrain::Type::EnemyUnit)
        {
            auto it = std::find_if(m_units.begin()
                    , m_units.end()
                    , [&](const Unit& unit)
                    { return unit.getCoordinates() == neighbour.coords; });
            assert(it != m_units.end() && "Did not find a proper neighbour, your map is wrong on the terrains");
            targets.push_back(&*it);
        }
    }
    return targets;
}

bool CombatManager::attack(const Unit& attacker)
{
    return attack(attacker, getPosibleTargets(attacker));
}

bool CombatManager::attack(const Unit& attacker, Unit& target)
{
    if (rng()%16 != 0)
    {
        target.getStats().healthPoints -= attacker.getStats().weaponAttack - target.getStats().weaponDefense;
        // TODO Trigger attack animation
        return true;
    }
        //TODO Trigger attack miss
        return false;
}

bool CombatManager::attack(const Unit& attacker, std::vector<Unit*> targets)
{
    if (targets.empty())
        return false;
    // busco la unidad con menor hp para atacar
    auto target = std::min_element(targets.begin(), targets.end(), [](const Unit* lhs, const Unit* rhs)
            { return lhs->getStats().healthPoints < rhs->getStats().healthPoints; });

    return attack(attacker, **target);
}
