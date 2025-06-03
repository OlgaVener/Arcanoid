#include "Brick.h"

Brick::Brick(float x, float y, int hitPoints)
    : hitPoints_(hitPoints) {
    shape_.setSize(sf::Vector2f(80.f, 30.f));
    shape_.setPosition(x, y);
    shape_.setOutlineThickness(1.f);
    shape_.setOutlineColor(sf::Color::Black);

    switch (hitPoints) 
    {
    case 1: shape_.setFillColor(sf::Color::Green); break;
    case 2: shape_.setFillColor(sf::Color::Yellow); break;
    case 3: shape_.setFillColor(sf::Color::Red); break;
    }
}

sf::Color Brick::getColor() const 
{
    return shape_.getFillColor();
}
void Brick::draw(sf::RenderWindow& window) const 
{
    if (!destroyed_) 
    {
        window.draw(shape_);
    }
}

void Brick::hit() 
{
    if (--hitPoints_ <= 0)
    {
        destroyed_ = true;
    }
}
sf::FloatRect Brick::getBounds() const 
{
    return shape_.getGlobalBounds();
}

bool Brick::isDestroyed() const 
{
    return destroyed_;
}

void Brick::destroy() 
{
    destroyed_ = true;
}