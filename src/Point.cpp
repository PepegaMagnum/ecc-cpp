//
// Created by shen on 17.09.2025.
//

#include <iostream>

class Point
{
    uint32_t m_x { 0 };
    uint32_t m_y { 0 };
public:

    Point() = default;

    Point(const uint32_t a_x, const uint32_t a_y) : m_x(a_x), m_y(a_y) {}
    void print() const
    {
        std::cout <<"(" << m_x << "," << m_y << ")";
    }
    uint32_t getX() const {return m_x;}
    void setX(uint32_t x) {m_x = x;}

    uint32_t getY() const {return m_y;}
    void setY(uint32_t y) {m_y = y;}



};
