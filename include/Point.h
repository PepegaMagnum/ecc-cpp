//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_POINT_H
#define ECC_CPP_POINT_H

class Point
{
    uint32_t m_x { 0 };
    uint32_t m_y { 0 };
public:

    Point() = default;

    Point(const uint32_t a_x, const uint32_t a_y);
    void print() const;

    uint32_t getX() const {return m_x;}
    void setX(const uint32_t x) {m_x = x;}

    uint32_t getY() const {return m_y;}
    void setY(const uint32_t y) {m_y = y;}

};

#endif //ECC_CPP_POINT_H