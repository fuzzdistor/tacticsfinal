#ifndef TF_COMBATMANAGER_HPP
#define TF_COMBATMANAGER_HPP

#include "map.hpp"
#include "unit.hpp"
#include "xorshift.hpp"

class CombatManager
{
public:
    CombatManager(const Map& map, std::vector<Unit>& units);
    bool tryAttack(const Unit& attacker);

private:
    bool attack(const Unit& attacker, Unit& target);
    bool attack(const Unit& attacker, std::vector<Unit*> targets);
    std::vector<Unit*> getPosibleTargets(const Unit& attacker);

    const Map& m_map;
    std::vector<Unit>& m_units;
    xorshift rng;
};

#endif // TF_COMBATMANAGER_HPP
