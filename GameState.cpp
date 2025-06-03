#include "GameState.h"
#include <string>

GameState::GameState(unsigned int width, unsigned int height) :
    window_(std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Arkanoid")),
    platform_(std::make_unique<Platform>(width / 2 - 50, height - 30, 100, 20)),
    ball_(std::make_unique<Ball>(width / 2, height / 2, 10))
{
    window_->setFramerateLimit(60);
}

void GameState::run() {
    sf::Clock clock;

    while (window_->isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }
}

void GameState::handleEvents() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_->close();
        }
    }
}

void GameState::update(float deltaTime) {
    handleInput();
    updateBall(deltaTime); 
    updateBallSpeed(deltaTime);
}

void GameState::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        platform_->move(-platformSpeed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        platform_->move(platformSpeed_);
    }

    // Platform boundary check
    sf::Vector2u winSize = window_->getSize();
    sf::FloatRect bounds = platform_->getGlobalBounds();
    if (bounds.left < 0) {
        platform_->setPosition(0);
    }
    if (bounds.left + bounds.width > winSize.x) {
        platform_->setPosition(winSize.x - bounds.width);
    }
}

void GameState::updateBall(float deltaTime) {
    ball_->update(deltaTime);
    checkCollisions();
}

void GameState::updateBallSpeed(float deltaTime) {
    ballSpeedChangeTimer_ += deltaTime;

    if (ballSpeedChangeTimer_ >= ballSpeedChangeInterval_) {
        ballSpeedChangeTimer_ = 0.0f;

        float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float range = maxBallSpeedMultiplier_ - minBallSpeedMultiplier_;
        float targetMultiplier = minBallSpeedMultiplier_ + random * range;

        currentBallSpeedMultiplier_ = currentBallSpeedMultiplier_ * 0.7f +
            targetMultiplier * 0.3f;

        ball_->setSpeedMultiplier(currentBallSpeedMultiplier_);
    }
}

void GameState::checkCollisions() {
    sf::Vector2u winSize = window_->getSize();
    sf::Vector2f ballPos = ball_->getPosition();
    float radius = ball_->getRadius();

    // Window boundaries
    if (ballPos.x <= 0 || ballPos.x + radius * 2 >= winSize.x) {
        ball_->reverseX();
    }
    if (ballPos.y <= 0) {
        ball_->reverseY();
    }

    // Platform collision
    if (ball_->getGlobalBounds().intersects(platform_->getGlobalBounds())) {
        ball_->reverseY();
        float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float bounceEffect = 0.9f + random * 0.2f;
        ball_->setSpeedMultiplier(ball_->getSpeedMultiplier() * bounceEffect);
    }
}

void GameState::render() {
    window_->clear(sf::Color::Black);

    std::string speedInfo = "Ball speed: " +
        std::to_string(baseBallSpeed_ * ball_->getSpeedMultiplier()).substr(0, 4) +
        " (base " + std::to_string((int)baseBallSpeed_) + ")";
    speedInfoText_.setString(speedInfo);

    window_->draw(platform_->getShape());
    window_->draw(ball_->getShape());
    window_->draw(speedInfoText_);
    window_->display();
}