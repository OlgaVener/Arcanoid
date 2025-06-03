#include "GameObject.h"

class Brick : public GameObject {
public:
    Brick(float x, float y, float width, float height, sf::Color color);

    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;
    bool isDestroyed() const;
    void destroy();

private:
    sf::RectangleShape shape_;
    bool destroyed_ = false;
};
