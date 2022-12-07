#ifndef UNITS_DATA
#define UNITS_DATA

#include "stats.hpp"
#include <array>
#include <string>

constexpr size_t data_units = 8;

const std::array<std::string, data_units> data_names
{
    "Marche",
    "Montblanc",
    "Reze",
    "Mila",
    "Borzo",
    "Villo",
    "Drago",
    "Fafnir",
};

const std::array<bool, data_units> data_playercontrolled
{
    true,
    true,
    true,
    true,
    false,
    false,
    false,
    false
};

const std::array<Stats, data_units> data_stats
{
    {
        //  mov lvl exp HP  mHP MP  mMP wpnAtk  wpnDef  magPow  magRes  spd  awareness
        {   4,  4,  0,  50, 50, 20, 20, 20,     5,      10,     10,     150, 8, },
        {   4,  4,  0,  50, 50, 20, 20, 20,     5,      10,     10,     145, 8, },
        {   4,  4,  0,  50, 50, 20, 20, 20,     6,      10,     10,     150, 8, },
        {   4,  4,  0,  50, 50, 20, 20, 20,     6,      10,     10,     145, 8, },
        {   4,  3,  0,  40, 40, 10, 10, 20,     5,      10,     10,     146, 8, },
        {   4,  3,  0,  40, 40, 10, 10, 20,     6,      10,     10,     130, 8, },
        {   4,  3,  0,  40, 40, 10, 10, 20,     7,      10,     10,     146, 8, },
        {   4,  3,  0,  40, 40, 10, 10, 20,     6,      10,     10,     130, 8, },
    },
};


#endif // UNITS_DATA
