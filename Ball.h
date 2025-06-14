#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float x, float y, float radius);

    void update(float deltaTime);
    void reset(float x, float y);
    void reverseX() { baseVelocity_.x = -baseVelocity_.x; }
    void reverseY() { baseVelocity_.y = -baseVelocity_.y; }
    void setSpeedMultiplier(float multiplier);
    void setPosition(float x, float y);
    void setColor(const sf::Color& color);


    sf::FloatRect getGlobalBounds() const { return shape_.getGlobalBounds(); }
    void setVelocity(const sf::Vector2f& velocity);
    sf::Vector2f getVelocity() const { return baseVelocity_; }

    const sf::CircleShape& getShape() const;
    const sf::Vector2f& getPosition() const;
    float getRadius() const;
    float getSpeedMultiplier() const;

private:
    void maintainCircleShape();
    sf::Vector2f position_;
    sf::Vector2f baseVelocity_;
    float speedMultiplier_;
    float radius_;
    sf::CircleShape shape_;
};