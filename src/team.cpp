#include "team.hpp"
#include "utils.hpp"

Team::Team(size_t max_size)
    : m_units()
{
    m_units.reserve(max_size);
}

