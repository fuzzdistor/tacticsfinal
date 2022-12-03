#ifndef UTILS_HPP
#define UTILS_HPP

#include "SFML/System/Vector2.hpp"
#include <functional>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

#ifndef NDEBUG
#include <source_location>
#include <iostream>
#include <chrono>
#endif // NDEBUG

using uint = unsigned int;


////////////////////////
// DEBUG MACROS
// DBG(var) - Writes variable source file and line, variable name and value
// D(var) - Simple write of variable value
// DBGCOND(cond, var) - Writes variable if condition is met
///////////////////////

#ifndef NDEBUG

#define TERM_WHITE "\033[38;2;255;255;255m"
#define TERM_GREY "\033[38;2;99;104;109m"
#define TERM_CYAN "\033[38;2;61;174;233m"
#define TERM_YELLOW "\033[38;2;253;220;75m"
#define TERM_ORANGE "\033[38;2;253;30;75m"
#define TERM_GREEN "\033[38;2;30;253;75m"

#define CONCAT(a, b) a ## b

#define _LOGONCE_IMPL(var, id) static bool CONCAT(var, id) = true;          \
                if (CONCAT(var, id)) { D(var); CONCAT(var, id) = false; }

#define _GETFLAGID __COUNTER__

#define DBG(var) (std::cout << TERM_GREY << '['                             \
                  << std::source_location::current().file_name() << ':'     \
                  << std::source_location::current().line() << "] "         \
                  << TERM_CYAN << (#var) << TERM_WHITE << " = "             \
                  << TERM_YELLOW << (var) << TERM_WHITE << std::endl)

#define D(var) (std::cout << var << std::endl)

#define DBGCOND(cond, var) if(cond) DBG(var)

#define DBGONCE(var) { _LOGONCE_IMPL(var, _GETFLAGID) }

#define TIMERINIT(var) auto var = std::chrono::high_resolution_clock::now()

#define RESET_TIMER(var) var = std::chrono::high_resolution_clock::now()

#define IN_MICROSECONDS(var) \
                    (std::chrono::duration_cast<std::chrono::microseconds>  \
                    (std::chrono::high_resolution_clock::now() - var)       \
                    .count())

#define DBGTIMER(var) DBG(IN_MICROSECONDS(var))

#else

#define DBG(var)
#define D(var)
#define DBGCOND(cond, var)
#define DBGONCE(var)
#define TIMERINIT(var)
#define RESET_TIMER(var)
#define DBGTIMER(var)

#endif // NDEBUG

//////////////////////////////////////////
// TYPELOGGING FOR DEBUG
// sf::Vector2<T>
/////////////////////////////////////////
#ifndef NDEBUG
template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vec)
{
    os << "Vector2<T> : { x: " << vec.x << ", y: " << vec.y << " }";
    return os;
}
#endif // NDEBUG

namespace fd
{
    constexpr auto hash = std::hash<std::string>{};
}

// Structured binding para separar x e y de un Vector2 en componentes individuales
template<typename T>
constexpr std::tuple<T, T> unwrap(const sf::Vector2<T>& vec)
{
    return {vec.x, vec.y};
}

template<typename T>
constexpr sf::Vector2<T> distanceVector(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
{
    return { lhs.x > rhs.x ? lhs.x - rhs.x : rhs.x - lhs.x,
        lhs.y > rhs.y ? lhs.y - rhs.y : rhs.y - lhs.y };
}

template<typename T>
constexpr bool masksMatch(T mask1, T mask2)
{
    return static_cast<uint>(mask1) & static_cast<uint>(mask2);
}

#endif // UTILS_HPP
