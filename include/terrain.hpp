#ifndef TF_TERRAIN_HPP
#define TF_TERRAIN_HPP

using uint = unsigned int;

namespace Terrain
{

    enum class [[nodiscard]] Type : uint
    {
        Ground = 1 << 0,
        Water = 1 << 1,
        None = 1 << 2,
        Obstacle = 1 << 3,
        Unit = 1 << 4,

        Walkable = Ground | Water | None,
        Castable = Ground | Water | Unit | None,
        All = 0xFFFFFFFF,
    };

    [[nodiscard]] constexpr bool isWalkable(Type type);

};

constexpr bool Terrain::isWalkable(Type type)
{
    return static_cast<uint>(type) & static_cast<uint>(Type::Walkable);
}

#endif // TF_TERRAIN_HPP
