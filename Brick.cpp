#include "Brick.h"

Brick::Brick(float x, float y, Type type)
    : type_(type) {
    // Установка размера и позиции
    shape_.setSize(sf::Vector2f(80.f, 30.f));
    shape_.setPosition(x, y);

    // Общие свойства для всех блоков
    shape_.setOutlineThickness(1.f);
    shape_.setOutlineColor(sf::Color::Black);

    // Установка свойств в зависимости от типа
    switch (type_) {
    case Type::Strong:
        hitPoints_ = 3;
        shape_.setFillColor(sf::Color(255, 165, 0)); // Оранжевый
        break;

    case Type::Glass:
        hitPoints_ = 1;
        shape_.setFillColor(sf::Color(200, 200, 255, 150)); // Прозрачный голубой
        break;

    default: // Type::Normal
        hitPoints_ = 1;
        shape_.setFillColor(sf::Color(100, 250, 100)); // Ярко-зелёный
    }
}

void Brick::hit() {
    if (--hitPoints_ <= 0) {
        destroyed_ = true;
    }
    else if (type_ == Type::Strong) {
        // Изменение цвета для прочного блока
        if (hitPoints_ == 2) {
            shape_.setFillColor(sf::Color(255, 200, 0)); // Жёлто-оранжевый
        }
        else if (hitPoints_ == 1) {
            shape_.setFillColor(sf::Color(255, 255, 0)); // Ярко-жёлтый
        }
    }
}

bool Brick::isDestroyed() const {
    return destroyed_;
}

void Brick::draw(sf::RenderWindow& window) const {
    if (!destroyed_) {
        window.draw(shape_);
    }
}

sf::FloatRect Brick::getBounds() const {
    return shape_.getGlobalBounds();
}

bool Brick::shouldBallBounce() const {
    // Мяч отскакивает только от не-стеклянных блоков
    return type_ != Type::Glass;
}