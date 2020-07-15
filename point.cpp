#include "point.hh"

Point& Point::operator+=(const Point& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Point& Point::operator-=(const Point& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Point& Point::operator/=(const int& rhs)
{
	x /= rhs;
	y /= rhs;
	return *this;
}

Point Point::operator-() const
{
	Point p;
	p.x = -x;
	p.y = -y;
	return p;
}

Point operator-(Point lhs, const Point& rhs)
{
	lhs -= rhs;
	return lhs;
}

bool operator==(const Point& lhs, const Point& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
