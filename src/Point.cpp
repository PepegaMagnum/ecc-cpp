//
// Created by shen on 17.09.2025.
//

#include <iostream>
#include "../include/Point.h"

Point::Point(const uint32_t a_x, const uint32_t a_y) : m_x(a_x), m_y(a_y)
{
    std::cout <<"(" << m_x << ", " << m_y << ") Point constructed\n";
}

void Point::print() const
{
    std::cout <<"(" << m_x << "," << m_y << ")";
}




