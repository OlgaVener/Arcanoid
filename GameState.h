#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Platform.h"
#include "Ball.h"
#include "Brick.h"

class GameState {
public:
    GameState(unsigned int width, unsigned int height);
    ~GameState() = default;

    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void handleInput();
    void updateBall(float deltaTime);
    void updateBallSpeed(float deltaTime);
    void checkCollisions();
    void initBricks();
    void checkWinCondition();
    void resetGame();

    std::vector<std::unique_ptr<Brick>> bricks_;
    bool gameWon_ = false;
    sf::Text winText_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<Platform> platform_;
    std::unique_ptr<Ball> ball_;

    sf::Text speedInfoText_;
    sf::Font font_;

    const float platformSpeed_ = 3.5f;
    const float baseBallSpeed_ = 250.0f;
    float ballSpeedChangeTimer_ = 0.0f;
    const float ballSpeedChangeInterval_ = 1.0f;
    float currentBallSpeedMultiplier_ = 1.0f;
    const float minBallSpeedMultiplier_ = 1.0f;
    const float maxBallSpeedMultiplier_ = 1.2f;
};