#include <SFML/Graphics.hpp>

class Platform 
{
public:
    Platform(float x, float y, float width, float height);

    void move(float offset);
    void setPosition(float x);
    const sf::RectangleShape& getShape() const;
    sf::FloatRect getGlobalBounds() const;

private:
    sf::Vector2f position_;
    sf::Vector2f size_;
    sf::RectangleShape shape_;
};
