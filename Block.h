#include "GameObject.h"

class Block : public GameObject {
public:
    Block(float x, float y, float width, float height, sf::Color color);
    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;
    bool IsDestroyed() const;
    void Destroy();
    sf::FloatRect GetBounds() const;

private:
    bool isDestroyed;
    sf::RectangleShape shape;
};