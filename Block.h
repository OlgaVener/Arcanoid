#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Block {
protected:
    sf::Sprite sprite_;
    int hitPoints_;
    bool destroyed_;

public:
    Block();
    virtual ~Block() = default;

    virtual void hit();
    virtual bool isDestroyed() const;
    virtual bool shouldBallBounce() const = 0;

    void setPosition(float x, float y);
    void setTexture(const sf::Texture& texture);

    virtual void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
};