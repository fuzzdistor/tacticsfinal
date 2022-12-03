#include "turnmanager.hpp"
#include <optional>

std::optional<Unit*> TurnManager::resolveTie(std::vector<Unit*> units)
{
    // lo más probable por mucho es que no haya elementos para resolver
    [[likely]] if (units.empty())
        return std::nullopt;

    // si existe un solo candidato para resolución, lo retorno
    if (units.size() == 1)
        return units.back();

    // retorno el elemento con menor valor de id
    return *std::min_element(units.begin(), units.end(),
            [] (const Unit* lhs, const Unit* rhs)
            { return lhs->getId() < rhs->getId(); });
}

std::optional<Unit*> TurnManager::getReadiedUnit()
{
    // preparo un vector para las posibles multiples unidades que cumplen
    // con la condición para tomar un turno
    std::vector<Unit*> readyUnits;

    for (auto& proxy : m_units)
    {
        if (proxy.counter >= 1000)
            readyUnits.push_back(proxy.unit);
    }

    return resolveTie(readyUnits);
}

Unit* TurnManager::advanceToNextTurn()
{
    while (true)
    {
        if (auto readiedUnit = getReadiedUnit())
            return *readiedUnit;

        advanceOneTick();

        resolveReadyUnits();
    }
}

void TurnManager::resolveReadyUnits()
{
    auto& element = *std::max_element(m_units.begin(), m_units.end(),
            [] (const TurnProxyUnit& lhs, TurnProxyUnit& rhs)
            { return lhs.counter < rhs.counter; });

    if (element.counter < 1000)
        return std::nullopt;

    // la reserva no se puede pasar de 500
    uint newReserve = std::min(maxCtElement.counter - 1000u, 500u);

    for (auto& proxy : m_units)
    {
        proxy.counter = std::min(proxy.counter - newReserve, 1000u);
        proxy.reserve = newReserve;
    }
}
