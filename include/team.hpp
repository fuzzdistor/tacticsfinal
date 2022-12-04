#ifndef TF_TEAM_HPP
#define TF_TEAM_HPP

#include "unit.hpp"
#include <cassert>
#include <string_view>
#include <type_traits>
#include <vector>

template<typename ... Args>
concept UnitConstructor = std::is_constructible_v<Unit, Args&&...>;

class Team
{
public:
    explicit Team(size_t max_size);

    // perfect-forwarding de argumentos para hacer emplace de la unidad sin
    // necesidad de hacer una copia ni un move
    constexpr Unit& emplaceUnit(UnitConstructor auto&& ... args);

    inline size_t size() const;
    inline auto begin() const;
    inline auto begin();
    inline auto end() const;
    inline auto end();
    inline Unit& operator[](size_t index);
    inline const Unit& at(size_t index) const;
    inline Unit& at(size_t index);

private:
    std::vector<Unit> m_units;
};

inline auto Team::begin() const
{
    return m_units.begin();
}

inline auto Team::begin()
{
    return m_units.begin();
}

inline auto Team::end() const
{
    return m_units.end();
}

inline auto Team::end()
{
    return m_units.end();
}

inline size_t Team::size() const
{
    return m_units.size();
}

constexpr Unit& Team::emplaceUnit(UnitConstructor auto&& ... args)
{
    return m_units.emplace_back(std::forward<decltype(args)>(args)...);
}

inline const Unit& Team::at(size_t index) const
{
    return m_units.at(index);
}

inline Unit& Team::at(size_t index)
{
    return m_units.at(index);
}

inline Unit& Team::operator[](size_t index)
{
    return m_units[index];
}
#endif // TF_TEAM_HPP
