#include "Block.h"

Block::Block(float x, float y, float width, float height, sf::Color color)
    : isDestroyed(false) {
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(color);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::White);
}

void Block::Update(float deltaTime) {}

void Block::Draw(sf::RenderWindow& window) {
    if (!isDestroyed) {
        window.draw(shape);
    }
}

bool Block::IsDestroyed() const {
    return isDestroyed;
}

void Block::Destroy() {
    isDestroyed = true;
}

sf::FloatRect Block::GetBounds() const {
    return shape.getGlobalBounds();
}