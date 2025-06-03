#include "Ball.h"

Ball::Ball(float x, float y, float radius)
    : position_{ x, y },
    baseVelocity_{ 250.0f, -250.0f },
    speedMultiplier_(1.0f),
    radius_(radius) {
    shape_.setRadius(radius_);
    shape_.setPosition(position_);
    shape_.setFillColor(sf::Color::Red);
}

void Ball::update(float deltaTime) {
    position_ += baseVelocity_ * deltaTime;
    shape_.setPosition(position_);
}

void Ball::setSpeedMultiplier(float multiplier) {
    speedMultiplier_ = multiplier;
}

// Реализации методов, которые были перенесены из заголовка
const sf::CircleShape& Ball::getShape() const {
    return shape_;
}

sf::FloatRect Ball::getGlobalBounds() const {
    return shape_.getGlobalBounds();
}

const sf::Vector2f& Ball::getPosition() const {
    return position_;
}

float Ball::getRadius() const {
    return radius_;
}

float Ball::getSpeedMultiplier() const {
    return speedMultiplier_;
}
