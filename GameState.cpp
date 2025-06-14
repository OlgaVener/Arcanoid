#include "GameState.h"
#include <string>

GameState::GameState(unsigned int width, unsigned int height) :
    window_(std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Arkanoid")),
    platform_(std::make_unique<Platform>(width / 2 - 50, height - 30, 100, 20)),
    ball_(std::make_unique<Ball>(width / 2, height / 2, 10))
{
    window_->setFramerateLimit(60);

    if (!font_.loadFromFile("arial.ttf")) {
        // Обработка ошибки загрузки шрифта
    }

    winText_.setFont(font_);
    winText_.setCharacterSize(30);
    winText_.setFillColor(sf::Color::White);

    initBricks();
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

        if (gameWon_ && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Y) {
                resetGame();
            }
            else if (event.key.code == sf::Keyboard::N) {
                window_->close();
            }
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

    sf::Vector2u winSize = window_->getSize();
    sf::FloatRect bounds = platform_->getGlobalBounds();

    if (bounds.left < 0.0f) {
        platform_->setPosition(0.0f, platform_->getPosition().y);
    }

    if (bounds.left + bounds.width > static_cast<float>(winSize.x)) {
        platform_->setPosition(
            static_cast<float>(winSize.x) - bounds.width,
            platform_->getPosition().y
        );
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

void GameState::initBricks() {
    bricks_.clear();

    const float startX = 50.f;
    const float startY = 50.f;
    const float padding = 5.f;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 10; ++col) {
            int hitPoints = 1;
            if (row >= 5) hitPoints = 3;
            else if (row >= 3) hitPoints = 2;

            bricks_.push_back(std::make_unique<Brick>(
                startX + col * (80.f + padding),
                startY + row * (30.f + padding),
                hitPoints
            ));
        }
    }
}

void GameState::checkCollisions() {
    sf::Vector2u winSize = window_->getSize();
    sf::Vector2f ballPos = ball_->getPosition();
    float radius = ball_->getRadius();

    // Столкновение со стенами
    if (ballPos.x <= 0 || ballPos.x + radius * 2 >= winSize.x) {
        ball_->reverseX();
    }
    if (ballPos.y <= 0) {
        ball_->reverseY();
    }

    // Столкновение с платформой
    if (ball_->getGlobalBounds().intersects(platform_->getGlobalBounds())) {
        ball_->reverseY();
        float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float bounceEffect = 0.9f + random * 0.2f;
        ball_->setSpeedMultiplier(ball_->getSpeedMultiplier() * bounceEffect);
    }

    // Столкновение с блоками
    for (auto& brick : bricks_) {
        if (!brick->isDestroyed() && ball_->getGlobalBounds().intersects(brick->getBounds())) {
            brick->hit();

            sf::FloatRect ballBounds = ball_->getGlobalBounds();
            sf::FloatRect brickBounds = brick->getBounds();

            float ballLeft = ballBounds.left;
            float ballRight = ballBounds.left + ballBounds.width;
            float ballTop = ballBounds.top;
            float ballBottom = ballBounds.top + ballBounds.height;

            float brickLeft = brickBounds.left;
            float brickRight = brickBounds.left + brickBounds.width;
            float brickTop = brickBounds.top;
            float brickBottom = brickBounds.top + brickBounds.height;

            float overlapLeft = ballRight - brickLeft;
            float overlapRight = brickRight - ballLeft;
            float overlapTop = ballBottom - brickTop;
            float overlapBottom = brickBottom - ballTop;

            bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
            bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
            bool fromTop = overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom;
            bool fromBottom = overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop;

            if (fromLeft || fromRight) {
                ball_->reverseX();
            }
            if (fromTop || fromBottom) {
                ball_->reverseY();
            }

            break;
        }
    }

    checkWinCondition();
}

void GameState::checkWinCondition() {
    bool allDestroyed = true;
    for (const auto& brick : bricks_) {
        if (!brick->isDestroyed()) {
            allDestroyed = false;
            break;
        }
    }

    if (allDestroyed) {
        gameWon_ = true;
        winText_.setString("Поздравляем! Вы выиграли!\nХотите сыграть еще? (Y/N)");
        winText_.setPosition(
            window_->getSize().x / 2 - winText_.getGlobalBounds().width / 2,
            window_->getSize().y / 2 - winText_.getGlobalBounds().height / 2
        );
    }
}

void GameState::resetGame() {
    initBricks();
    ball_->reset(window_->getSize().x / 2, window_->getSize().y / 2);
    platform_->setPosition(window_->getSize().x / 2 - 50, window_->getSize().y - 30);
    gameWon_ = false;
}

void GameState::render() {
    window_->clear(sf::Color::Black);

    for (const auto& brick : bricks_) {
        brick->draw(*window_);
    }

    window_->draw(ball_->getShape());
    window_->draw(platform_->getShape());

    if (gameWon_) {
        window_->draw(winText_);
    }

    window_->display();
}