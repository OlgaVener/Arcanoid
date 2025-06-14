#include <SFML/Graphics.hpp>

class Brick {
public:
    enum class Type {
        Normal,    // Обычный (1 удар)
        Strong,    // Прочный (3 удара)
        Glass      // Стеклянный (1 удар, мяч не отскакивает)
    };

    Brick(float x, float y, Type type);

    void hit();
    bool isDestroyed() const;
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool shouldBallBounce() const;
    Type getType() const { return type_; }

private:
    sf::RectangleShape shape_;
    Type type_;
    int hitPoints_;
    bool destroyed_ = false;
};
