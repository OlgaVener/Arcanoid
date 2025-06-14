#include "Brick.h"

Brick::Brick() :
    type_(Type::Normal),
    hitPoints_(1),
    destroyed_(false) {}

void Brick::setPosition(float x, float y) {
    sprite_.setPosition(x, y);
}

void Brick::setType(Type type) {
    type_ = type;
    hitPoints_ = (type == Type::Strong) ? 3 : 1;
}

void Brick::setTexture(const sf::Texture& texture) {
    sprite_.setTexture(texture);
}

void Brick::hit() {
    if (--hitPoints_ <= 0) {
        destroyed_ = true;
    }
}

bool Brick::isDestroyed() const {
    return destroyed_;
}

bool Brick::shouldBallBounce() const {
    return type_ != Type::Glass;
}

void Brick::draw(sf::RenderWindow& window) const {
    if (!destroyed_) {
        window.draw(sprite_);
    }
}

const sf::Sprite& Brick::getSprite() const {
    return sprite_;
}

sf::FloatRect Brick::getBounds() const {
    return sprite_.getGlobalBounds();
}

Brick::Type Brick::getType() const {
    return type_;
}