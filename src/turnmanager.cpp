#include "turnmanager.hpp"
#include <optional>

std::optional<Unit*> TurnManager::checkForQuick()
{
    std::vector<Unit*> a;

    // junto todos los elementos que tengan quick
    for (auto& turnUnit : m_units)
    {
        if (turnUnit.unit->getStatus() == Status::Quick)
            a.push_back(turnUnit.unit);
    }

    return resolveTie(a);
}

