#ifndef TF_TURNMANAGER_HPP
#define TF_TURNMANAGER_HPP

#include "unit.hpp"

struct TurnUnit
{
    Unit* unit;
    uint counter;
    uint reserve;
};

class TurnManager
{
public:
    TurnManager();

    Unit* getTurn();
    bool tick();
    void advanceSpeed();
    std::optional<Unit*> checkForQuick();
    constexpr std::optional<Unit*> resolveTie(std::vector<Unit*> units);

private:

    std::vector<TurnUnit> m_units;
};

constexpr std::optional<Unit*> TurnManager::resolveTie(std::vector<Unit*> units)
{
    // lo más probable por mucho es que no haya elementos en units
    [[likely]] if (units.empty())
        return std::nullopt;
    if (units.size() == 1)
        return units.back();

    // retorno el elemento con menor valor de id
    return *std::min_element(units.begin(), units.end(),
            [](const Unit* lhs, const Unit* rhs)
            { return lhs->getId() < rhs->getId(); });
}

#endif // TF_TURNMANAGER_HPP
