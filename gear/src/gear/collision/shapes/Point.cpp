#include "Point.h"

gear::Point::Point(const gear::Vector<float, 2> &position) : Shape(ShapeType::POINT), position(position) {}

