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
    void checkLoseCondition();
    void initBricks();
    void checkWinCondition();
    void resetGame();
    void showLoseScreen();
    void handleLoseScreenInput();
    void showWinScreen();
    void handleWinScreenInput();

    std::vector<std::unique_ptr<Brick>> bricks_;
    bool gameWon_ = false;
    bool gameLost_ = false;
    sf::Text winText_;
    sf::Text loseText_;
    sf::Font font_;
    sf::Text speedInfoText_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<Platform> platform_;
    std::unique_ptr<Ball> ball_;


    const float platformSpeed_ = 5.0f;
    const float baseBallSpeed_ = 280.f;
    const float minBallSpeedMultiplier_ = 0.8f;
    const float maxBallSpeedMultiplier_ = 1.5f;
    float ballSpeedChangeTimer_ = 0.0f;
    const float ballSpeedChangeInterval_ = 1.0f;
    float currentBallSpeedMultiplier_ = 1.0f;
};