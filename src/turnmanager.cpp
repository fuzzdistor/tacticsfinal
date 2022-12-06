#include "turnmanager.hpp"
#include "unit.hpp"
#include "utils.hpp"
#include <cassert>
#include <imgui.h>
#include <optional>

TurnProxyUnit::TurnProxyUnit(const Unit* unit)
    : m_unit(unit)
    , counter(0)
    , reserve(0)
{
}

void TurnProxyUnit::advanceCounter()
{
    switch(m_unit->getStatus())
    {
        case Status::Haste:
            counter += m_unit->getStats().speed * 2u;
            break;
        case Status::Slow:
            counter += m_unit->getStats().speed / 2u;
            break;
        default:
            counter += m_unit->getStats().speed;
    }
    counter += reserve;
    if (counter >= 1000)
        notifyReadyUnit(this);

    reserve = 0;
}

void TurnManager::registerUnit(const Unit* unit)
{
    m_units.emplace_back(unit);
    D("Registered unit " << unit->getName());
    m_units.back().notifyReadyUnit =
        [&](const TurnProxyUnit* turn){
            D("Notifying Ready Unit!");
            m_readyUnits.emplace_back(turn); };
}

Unit* TurnManager::resolveTie(const std::vector<Unit*>& units) const
{
    // no debería llamar resolveTie con un vector vacío
    assert(units.empty() == false);

    // si existe un solo candidato para resolución, lo retorno
    if (units.size() == 1)
        return units.back();

    // retorno el elemento con menor valor de id
    return *std::min_element(units.begin(), units.end(),
            [] (const Unit* lhs, const Unit* rhs)
            { return lhs->getId() < rhs->getId(); });
}

TurnProxyUnit& TurnManager::getHighestCtUnit()
{
    return *std::max_element(m_units.begin(), m_units.end(),
            [] (const TurnProxyUnit& lhs, TurnProxyUnit& rhs)
            { return lhs.counter < rhs.counter; });
}

void TurnManager::advanceOneTick()
{
    for (auto& proxy : m_units)
    {
        proxy.advanceCounter();
    }
}

const Unit* TurnManager::getNextUnitAdvance()
{
    while (true)
    {
        if (auto& maxCtElement = getHighestCtUnit(); maxCtElement.counter >= 1000)
        {
            D(maxCtElement.m_unit->getName() << " takes its turn!");
            return maxCtElement.m_unit;
        }
        m_readyUnits.clear();
        advanceOneTick();

        if (!m_readyUnits.empty())
            computeUnitsTurnVariables();
    }
}

void TurnManager::computeUnitsTurnVariables()
{
    auto& maxCtElement = getHighestCtUnit();

    assert(maxCtElement.counter >= 1000);

    // la reserva no se puede pasar de 500
    uint newReserve = std::min(maxCtElement.counter - 1000u, 500u);

    for (auto& proxy : m_units)
    {
        proxy.counter = std::min(proxy.counter - newReserve, 1000u);
        proxy.reserve = newReserve;
    }
}

void TurnManager::takeCtFromUnit(const Unit& unit, ActionTaken action)
{
    auto it = std::find_if(m_units.begin(), m_units.end(),
            [&](const auto& unitProxy){ return unitProxy.m_unit == &unit; });

    assert(it != m_units.end());
    assert(it->counter == 1000);

    D(TERM_GREEN << "takingCt!");
    D("unit ct: " << it->counter);
    D("unit reserve: " << it->reserve);
    D("action: " << static_cast<uint>(action));

    switch (action)
    {
        case ActionTaken::None:
            it->counter -= 500;
            break;
        case ActionTaken::Moved:
            it->counter -= 800;
            break;
        case ActionTaken::UsedAction:
            it->counter -= 700;
            break;
        case ActionTaken::MovedAndAction:
            it->counter -= 1000;
            break;
    }
    D("New unit ct: " << it->counter);
    D("New unit reserve: " << it->reserve);
}
