#include "Attacks.h"

#include <algorithm>

int getDirectionCode(Direction d);

Attacks::Attacks()
    : m_attacks(0)
{
    std::fill(m_directions, m_directions + 8, false);
}
void Attacks::add(Direction d)
{
    m_attacks++;
    int code = getDirectionCode(d);
    if (code != -1)
        m_directions[code] = true;
}
bool Attacks::isDirectionAttacked(Direction d) const
{
    int code = getDirectionCode(d);
    return code != -1 ? m_directions[code] : false;
}

/*   321
*    4S0
*    567
*/
int getDirectionCode(Direction d)
{
    if (d.r == 0 && d.f == 1) return 0;
    if (d.r == 1 && d.f == 1) return 1;
    if (d.r == 1 && d.f == 0) return 2;
    if (d.r == 1 && d.f == -1) return 3;
    if (d.r == 0 && d.f == -1) return 4;
    if (d.r == -1 && d.f == -1) return 5;
    if (d.r == -1 && d.f == 0) return 6;
    if (d.r == -1 && d.f == 1) return 7;
    return -1;
}
