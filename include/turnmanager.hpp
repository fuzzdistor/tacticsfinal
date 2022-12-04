#ifndef TF_TURNMANAGER_HPP
#define TF_TURNMANAGER_HPP

#include <functional>

class Unit;

struct TurnProxyUnit
{
    explicit TurnProxyUnit(const Unit* unit);
    void advanceCounter();

    const Unit* m_unit;
    std::function<void(TurnProxyUnit*)> notifyReadyUnit;
    uint counter;
    uint reserve;
};

class TurnManager
{
public:
    enum class ActionTaken
    {
        None,
        Moved,
        UsedAction,
        MovedAndAction,
    };

    TurnManager() = default;

    void registerUnit(const Unit* unit);

    /// @brief Avanza hasta que se resuelve quien es la siguiente unidad en
    /// tomar un turno.
    /// @return La unidad a la que le toca tomar el turno.
    [[nodiscard]] const Unit* getNextUnitAdvance();
    [[nodiscard]] TurnProxyUnit& getHighestCtUnit();

    void takeCtFromUnit(const Unit* unit, ActionTaken action);

private:
    void advanceOneTick();
    [[nodiscard]] Unit* resolveTie(const std::vector<Unit*>& units) const;
    void computeUnitsTurnVariables();

    std::vector<TurnProxyUnit> m_units;
    std::vector<const TurnProxyUnit*> m_readyUnits;
};

#endif // TF_TURNMANAGER_HPP

