#pragma once
#include <math.h>
#include <algorithm>

struct checkpoint
{
    float m_pos_x = 0.0f;
    float m_pos_y = 0.0f;
    float m_width = 1.0f;
    float m_height = 1.0f;


    bool intersect2(checkpoint& other)
    {
        return (fabs(m_pos_x - other.m_pos_x) * 2.0f < (m_width + other.m_width)) &&
            (fabs(m_pos_y - other.m_pos_y) * 2.0f < (m_height + other.m_height));
    }
    float intersectDown(checkpoint& other)
    {
        if (fabs(m_pos_x - other.m_pos_x) * 2.0f >= (m_width + other.m_width) || m_pos_y > other.m_pos_y)
            return 0.0f;
        return std::min<float>(0.0f, other.m_pos_y - (other.m_height / 2.0f) - m_pos_y - (m_height / 2.0f));
    }

    float intersectUP(checkpoint& other)
    {
        if (fabs(m_pos_x - other.m_pos_x) * 2.0f >= (m_width + other.m_width) || m_pos_y > other.m_pos_y)
            return 0.0f;
        return std::min<float>(0.0f, other.m_pos_y - (other.m_height / 2.0f) - m_pos_y - (m_height / 2.0f));
    }
    float intersectSide(checkpoint& other)
    {
        if (fabs(m_pos_x - other.m_pos_x) * 2.0f >= (m_width + other.m_width) || m_pos_y > other.m_pos_y)
            return 0.0f;
        return std::min<float>(0.0f, other.m_pos_y - (other.m_height / 2.0f) - m_pos_y - (m_height / 2.0f));
    }
    /** Detects a horizontal intersection this Box is beside a target box (other).
    *   and reports the adjustment offset so that the two boxes are separated.
    *
    *   \param other is the second (target) Box to test collision on the horizontal axis with.
    *   \return the offset this Box needs to move in the x axis, so that it does not collide
    *   anymore with the target Box (other), or 0.0f if no collision is found.
    */
    float intersectSideways(checkpoint& other)
    {
        if (fabs(m_pos_y - other.m_pos_y) * 2.0f >= (m_width + other.m_width))
            return 0.0f;
        if (m_pos_x > other.m_pos_x)
            return std::max<float>(0.0f, other.m_pos_x + (other.m_width / 2.0f) - m_pos_x + (m_width / 2.0f));
        else
            return std::min<float>(0.0f, other.m_pos_x - (other.m_width / 2.0f) - m_pos_x - (m_width / 2.0f));
    }


    /** Default ctor
    */
    checkpoint() {}

    /** Basic Box ctor.
    *   \param x is the x coordinate of the box center
    *   \param y is the y coordinate of the box center
    *   \param w is the width of the box
    *   \param h is the height of the box
    */
    checkpoint(float x, float y, float w, float h)
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h) {}
};