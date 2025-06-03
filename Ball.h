#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float x, float y, float radius);

    void update(float deltaTime);
    void reset(float x, float y);
    void reverseX() { baseVelocity_.x = -baseVelocity_.x; }
    void reverseY() { baseVelocity_.y = -baseVelocity_.y; }
    void setSpeedMultiplier(float multiplier);

    const sf::CircleShape& getShape() const;
    sf::FloatRect getGlobalBounds() const;
    const sf::Vector2f& getPosition() const;
    float getRadius() const;
    float getSpeedMultiplier() const;

private:
    sf::Vector2f position_;
    sf::Vector2f baseVelocity_;
    float speedMultiplier_;
    float radius_;
    sf::CircleShape shape_;
};