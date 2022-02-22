#include "Point.h"

gear::Point::Point(const gear::Vector<double, 2> &position) : Shape(ShapeType::POINT), position(position) {}

