#ifndef UNITS_DATA
#define UNITS_DATA

#include "stats.hpp"
#include <array>
#include <string>

const std::array<std::string, 4> data_names
{
    "Marche",
    "Montblanc",
    "Borzo",
    "Villo",
};

const std::array<bool, 4> data_playercontrolled
{
    true,
    true,
    false,
    false
};

const std::array<Stats, 4> data_stats
{
    {{
    5, // movement
    0, // level
    0, // experiencePoints
    50, // healthPoints
    50, // maxHealthPoints
    20, // magicPoints
    20, // maxMagicPoints
    10, // weaponAttack
    0, // weaponDefense
    10, // magicPower
    10, // magicResistance
    150, // speed
    10, // awareness
    },
    {
    5, // movement
    0, // level
    0, // experiencePoints
    50, // healthPoints
    50, // maxHealthPoints
    20, // magicPoints
    20, // maxMagicPoints
    10, // weaponAttack
    0, // weaponDefense
    10, // magicPower
    10, // magicResistance
    145, // speed
    10, // awareness
    },
    {
    5, // movement
    0, // level
    0, // experiencePoints
    40, // healthPoints
    40, // maxHealthPoints
    10, // magicPoints
    10, // maxMagicPoints
    4, // weaponAttack
    0, // weaponDefense
    10, // magicPower
    10, // magicResistance
    130, // speed
    10, // awareness
    },
    {
    5, // movement
    0, // level
    0, // experiencePoints
    40, // healthPoints
    40, // maxHealthPoints
    10, // magicPoints
    10, // maxMagicPoints
    4, // weaponAttack
    0, // weaponDefense
    10, // magicPower
    10, // magicResistance
    130, // speed
    10, // awareness
    }},
};


#endif // UNITS_DATA
