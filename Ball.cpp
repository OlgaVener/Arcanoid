#include "Ball.h"
#include <algorithm>

Ball::Ball(float x, float y, float radius)
    : position_{ x, y },
    baseVelocity_{ 250.0f, -250.0f },
    speedMultiplier_(1.0f),
    radius_(radius) {
    shape_.setRadius(radius_);
    shape_.setPosition(position_);
    shape_.setFillColor(sf::Color::Red);
}
// ���������� �������, ������� ���� ���������� �� ���������
const sf::CircleShape& Ball::getShape() const {
    return shape_;
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

void Ball::setVelocity(const sf::Vector2f& velocity) {
    // ��������� �����������, �� ������������ ��������
    float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length > 0) {
        float minSpeed = 200.f;
        float maxSpeed = 500.f;
        float targetSpeed = std::max(minSpeed, std::min(maxSpeed, length));
        baseVelocity_ = (velocity / length) * targetSpeed;
    }
}

void Ball::maintainCircleShape() {
    float radius = shape_.getRadius();
    shape_.setRadius(radius);
    shape_.setOrigin(radius, radius);
}

void Ball::update(float deltaTime) {
    position_ += baseVelocity_ * deltaTime;
    
    // ��������� ������� �����
    shape_.setRadius(radius_);
    shape_.setOrigin(radius_, radius_);
    shape_.setPosition(position_);
}


void Ball::setSpeedMultiplier(float multiplier) {
    speedMultiplier_ = multiplier;
}

void Ball::setPosition(float x, float y) {
    position_.x = x;
    position_.y = y;
    shape_.setPosition(position_);
}

void Ball::setColor(const sf::Color& color) {
    shape_.setFillColor(color);
}

void Ball::reset(float x, float y) {
    position_.x = x;
    position_.y = y;
    shape_.setPosition(position_);
    baseVelocity_ = sf::Vector2f(250.f, -250.f);
    speedMultiplier_ = 1.0f;
    shape_.setFillColor(sf::Color::Red); // ���������� ����
}



