#ifndef TF_STATS_HPP
#define TF_STATS_HPP

using uint = unsigned int;

struct Stats
{
    uint movement = 0;
    uint level = 0;
    uint experiencePoints = 0;

    uint healthPoints = 0;
    uint maxHealthPoints = 0;
    uint magicPoints = 0;
    uint maxMagicPoints = 0;

    uint weaponAttack = 0;
    uint weaponDefense = 0;
    uint magicPower = 0;
    uint magicResistance = 0;
    uint speed = 0;
};

#endif // TF_STATS_HPP
