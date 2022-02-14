#include "Rect.h"

gear::Rect::Rect(void) : Shape(ShapeType::RECT), top_Left{0, 0}, bottom_Right{0, 0} {}

gear::Rect::Rect(const gear::Vector<float, 2> &top_Left, const gear::Vector<float, 2> &bottom_Right)
    : Shape(ShapeType::RECT), top_Left(top_Left), bottom_Right(bottom_Right) {}