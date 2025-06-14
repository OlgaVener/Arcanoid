#include <SFML/Graphics.hpp>

class Brick {
public:
    enum class Type { Normal, Strong, Glass };

    Brick();

    void setPosition(float x, float y);
    void setType(Type type);
    void setTexture(const sf::Texture& texture);

    void hit();
    bool isDestroyed() const;
    bool shouldBallBounce() const;

    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const;  // Добавлен новый метод
    sf::FloatRect getBounds() const;
    Type getType() const;

private:
    Type type_;
    int hitPoints_;
    bool destroyed_;
    sf::Sprite sprite_;
};