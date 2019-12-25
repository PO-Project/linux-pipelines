#ifndef POINT_H_
#define POINT_H_

class Point
{
public:
    int x, y;
    Point operator+(const Point &v) const
    {
        return Point{x + v.x, y + v.y};
    }
    Point operator-(const Point &v) const
    {
        return Point{x - v.x, y - v.y};
    }
    Point operator*(const int &s) const
    {
        return Point{x * s, y * s};
    }
    Point operator/(const int &s) const
    {
        return Point{x / s, y / s};
    }
};

#endif /* !POINT_H_ */
