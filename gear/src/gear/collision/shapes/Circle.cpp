#include "Circle.h"

gear::Circle::Circle(const gear::Vector<float, 2> &position, float radius) : Shape(ShapeType::CIRCLE), position(position), radius(radius) {}