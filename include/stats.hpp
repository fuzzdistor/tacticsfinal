#ifndef TF_STATS_HPP
#define TF_STATS_HPP

using uint = unsigned int;

struct Stats
{
    uint movement = 0;
    uint level = 0;
    uint experiencePoints = 0;

    float healthPoints = 0;
    float maxHealthPoints = 0;
    float magicPoints = 0;
    float maxMagicPoints = 0;

    float weaponAttack = 0;
    float weaponDefense = 0;
    float magicPower = 0;
    float magicResistance = 0;
    float speed = 0;
};

#endif // TF_STATS_HPP
