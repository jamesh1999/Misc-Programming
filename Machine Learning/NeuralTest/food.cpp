#include "food.h"

Food::Food()
{
    m_amount = 20.0 * static_cast<double>(rand()) / RAND_MAX;
    m_position[0] = static_cast<double>(rand()) / RAND_MAX;
    m_position[1] = static_cast<double>(rand()) / RAND_MAX;
}
