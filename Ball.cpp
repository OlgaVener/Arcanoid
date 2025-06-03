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

void Ball::reset(float x, float y) {
    position_ = sf::Vector2f(x, y);
    baseVelocity_ = sf::Vector2f(8.0f, -8.0f);
    speedMultiplier_ = 1.0f;
    shape_.setPosition(position_);
}

// ���������� �������, ������� ���� ���������� �� ���������
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
