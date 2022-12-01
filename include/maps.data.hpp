#ifndef MAPS_DATA
#define MAPS_DATA

#include "map.hpp"

constexpr MapData m = {
    12, 9,
    "wwww........" \
    "..x........." \
    "..xx........" \
    "...u........" \
    "............" \
    "....xx......" \
    "............" \
    "......xxx..." \
    "............" ,
    "media/tilemaps/simple.png"
};

static_assert(m.map_string[m.width*m.height-1]);
static_assert(!m.map_string[m.width*m.height]);


#endif // MAPS_DATA
