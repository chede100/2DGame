#ifndef TDG_INFORMATION_H_INCLUDED
#define TDG_INFORMATION_H_INCLUDED

enum EntityTyp
{
    Character, Object, Player, nothing
};

enum Direction
{
    north_west, north, north_east, east, west, south_west, south, south_east, noDirection
};

enum MovementStatus
{
    s_north, s_north_east, s_east, s_south_east, s_south, s_south_west, s_west, s_north_west,
    m_north, m_north_east, m_east, m_south_east, m_south, m_south_west, m_west, m_north_west,
    noStatus
};

#endif // TDG_INFORMATION_H_INCLUDED
