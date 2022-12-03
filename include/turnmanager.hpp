#ifndef TF_TURNMANAGER_HPP
#define TF_TURNMANAGER_HPP

#include "unit.hpp"

struct TurnProxyUnit
{
    Unit* unit;
    uint counter;
    uint reserve;
};

class TurnManager
{
public:
    TurnManager();

    /// @brief Avanza hasta que se resuelve quien es la siguiente unidad en
    /// tomar un turno.
    /// @return La unidad a la que le toca tomar el turno.
    Unit* advanceToNextTurn();

private:
    constexpr void advanceOneTick();
    std::optional<Unit*> getReadiedUnit();
    std::optional<Unit*> resolveTie(std::vector<Unit*> units);

    /// @brief Determina si hay unidades que cumplen con la condición para
    /// tomar un turno y maneja los puntajes de contador de manera acorde.
    /// @return true si al menos una unidad cumple la condición para tomar un
    /// turno, sino false.
    void resolveReadyUnits();

    std::vector<TurnProxyUnit> m_units;
};

constexpr void TurnManager::advanceOneTick()
{
    for (auto& proxy : m_units)
    {
        switch(proxy.unit->getStatus())
        {
            case Status::Haste:
                proxy.counter += proxy.unit->getStats().speed * 2u;
                break;
            case Status::Slow:
                proxy.counter += proxy.unit->getStats().speed / 2u;
                break;
            default:
                proxy.counter += proxy.unit->getStats().speed;
        }

        proxy.counter += proxy.reserve;
        proxy.reserve = 0;
    }
}

#endif // TF_TURNMANAGER_HPP

