#ifndef POINT_H
#define POINT_H

struct Point
{
	Point() : x(0), y(0) {}
	Point(int point_x, int point_y) : x(point_x), y(point_y) {}

	int x, y;

	Point& operator+=(const Point& rhs);
	Point& operator-=(const Point& rhs);
	Point& operator/=(const int& rhs);
	Point operator-() const;
};

Point operator-(Point lhs, const Point& rhs);
bool operator==(const Point& lhs, const Point& rhs);

const Point UP = Point(0,  1);
const Point DOWN = Point(0, -1);

#endif // POINT_H
