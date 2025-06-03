#include "Brick.h"

Brick::Brick(float x, float y, float width, float height, sf::Color color) {
    shape_.setSize(sf::Vector2f(width, height));
    shape_.setPosition(x, y);
    shape_.setFillColor(color);
    shape_.setOutlineThickness(1);
    shape_.setOutlineColor(sf::Color::Black);
}

void Brick::draw(sf::RenderWindow& window) const {
    if (!destroyed_) {
        window.draw(shape_);
    }
}

sf::FloatRect Brick::getBounds() const {
    return shape_.getGlobalBounds();
}

bool Brick::isDestroyed() const {
    return destroyed_;
}

void Brick::destroy() {
    destroyed_ = true;
}