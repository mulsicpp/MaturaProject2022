#include "Circle.h"

gear::Circle::Circle(const gear::Vector<double, 2> &position, double radius) : Shape(ShapeType::CIRCLE), position(position), radius(radius) {}