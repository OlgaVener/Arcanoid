#include <SFML/Graphics.hpp>

class Brick {
public:
    Brick(float x, float y, int hitPoints);

    void draw(sf::RenderWindow& window) const;
    void setColor(const sf::Color& color);
    void setOutline(float thickness, const sf::Color& color);
    sf::FloatRect getBounds() const;
    void destroy();
    bool isDestroyed() const;
    void hit();
    sf::Color getColor() const;

private:
    sf::RectangleShape shape_;
    int hitPoints_;
    bool destroyed_ = false;
};
