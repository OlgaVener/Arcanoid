#include "Platform.h"

Platform::Platform(float x, float y, float width, float height)
    : position_{ x, y }, size_{ width, height } 
{
    shape_.setSize(size_);
    shape_.setPosition(position_);
    shape_.setFillColor(sf::Color::White);
}

void Platform::move(float offset) 
{
    position_.x += offset;
    shape_.setPosition(position_);
}

void Platform::setPosition(float x) 
{
    position_.x = x;
    shape_.setPosition(position_);
}

const sf::RectangleShape& Platform::getShape() const 
{
    return shape_;
}

sf::FloatRect Platform::getGlobalBounds() const 
{
    return shape_.getGlobalBounds();
}