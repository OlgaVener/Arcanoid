#include <SFML/Graphics.hpp>

class Brick {
public:
    Brick(float x, float y, int hitPoints);

    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isDestroyed() const;
    void hit();
    sf::Color getColor() const;

private:
    sf::RectangleShape shape_;
    int hitPoints_;
    bool destroyed_ = false;
};
