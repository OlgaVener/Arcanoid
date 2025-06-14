#include "GameState.h"
#include <algorithm>
#include <cmath>

GameState::GameState(unsigned int width, unsigned int height) :
    window_(std::make_unique<sf::RenderWindow>(
        sf::VideoMode(800, 600),
        "Arkanoid",
        sf::Style::Titlebar | sf::Style::Close)),
    platform_(std::make_unique<Platform>(400 - 50, 600 - 50, 100, 20)),
    ball_(std::make_unique<Ball>(400, 500, 10.f))
{
    // Настройка окна
    window_->setFramerateLimit(60);
    window_->setVerticalSyncEnabled(true);

    // Инициализация мяча
    ball_->setVelocity(sf::Vector2f(180.f, -220.f));
    ball_->setColor(sf::Color::Red);

    // Загрузка шрифта
    if (!font_.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    // Настройка текста проигрыша
    loseText_.setFont(font_);
    loseText_.setCharacterSize(40);
    loseText_.setFillColor(sf::Color::White);
    loseText_.setString(
        "Game Over!\n"
        "Try again?\n"
        "(Y - Yes, N - No)"
    );
    centerText(loseText_);

    // Настройка текста победы
    winText_.setFont(font_);
    winText_.setCharacterSize(40);
    winText_.setFillColor(sf::Color::White);
    winText_.setString(
        "Congratulations! You won!\n"
        "Play again?\n"
        "(Y - Yes, N - No)"
    );
    centerText(winText_);

    // Инициализация блоков
    initBricks();
}

void GameState::centerText(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2, bounds.height / 2);
    text.setPosition(400, 300);
}

void GameState::run() {
    sf::Clock clock;

    while (window_->isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        if (gameWon_) {
            handleWinScreenInput();
            showWinScreen();
        }
        else if (gameLost_) {
            handleLoseScreenInput();
            showLoseScreen();
        }
        else {
            handleEvents();
            update(deltaTime);
            render();
        }
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
    checkGameConditions();
}

void GameState::handleInput() {
    // Движение платформы
    float direction = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction += 1.f;
    }

    platform_->move(direction * platformSpeed_);

    // Ограничение движения платформы
    sf::FloatRect bounds = platform_->getGlobalBounds();
    if (bounds.left < 0.f) {
        platform_->setPosition(0.f, bounds.top);
    }
    else if (bounds.left + bounds.width > 800.f) {
        platform_->setPosition(800.f - bounds.width, bounds.top);
    }
}

void GameState::updateBall(float deltaTime) {
    ball_->update(deltaTime);
    checkCollisions();
}

void GameState::updateBallSpeed(float deltaTime) {
    ballSpeedChangeTimer_ += deltaTime;

    if (ballSpeedChangeTimer_ >= ballSpeedChangeInterval_) {
        ballSpeedChangeTimer_ = 0.f;
        currentBallSpeedMultiplier_ = std::min(
            maxBallSpeedMultiplier_,
            currentBallSpeedMultiplier_ * 1.05f
        );

        // Визуальная индикация ускорения
        float ratio = (currentBallSpeedMultiplier_ - minBallSpeedMultiplier_) /
            (maxBallSpeedMultiplier_ - minBallSpeedMultiplier_);
        int red = 100 + static_cast<int>(155 * ratio);
        ball_->setColor(sf::Color(red, 100, 100));

        ball_->setSpeedMultiplier(currentBallSpeedMultiplier_);
    }
}

void GameState::initBricks() {
    bricks_.clear();

    const int rows = 5;
    const int cols = 8;
    const float width = 90.f;
    const float height = 25.f;
    const float startX = (800 - (cols * (width + 5.f))) / 2.f;
    const float startY = 50.f;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Brick::Type type = Brick::Type::Normal;

            if (row == 0 && col % 3 == 0) type = Brick::Type::Strong;
            else if (row == 2 && col % 4 == 0) type = Brick::Type::Glass;

            bricks_.push_back(std::make_unique<Brick>(
                startX + col * (width + 5.f),
                startY + row * (height + 5.f),
                type
            ));
        }
    }
}

void GameState::checkCollisions() {
    // Границы экрана
    sf::Vector2f ballPos = ball_->getPosition();
    float radius = ball_->getRadius();

    if (ballPos.x - radius <= 0 || ballPos.x + radius >= 800) {
        ball_->reverseX();
    }
    if (ballPos.y - radius <= 0) {
        ball_->reverseY();
    }

    // Столкновение с платформой
    if (ball_->getGlobalBounds().intersects(platform_->getGlobalBounds())) {
        float platformCenter = platform_->getPosition().x + platform_->getGlobalBounds().width / 2;
        float hitPos = (ball_->getPosition().x - platformCenter) / (platform_->getGlobalBounds().width / 2);

        sf::Vector2f newVel;
        newVel.x = hitPos * 300.f;
        newVel.y = -sqrt(300.f * 300.f - newVel.x * newVel.x);
        ball_->setVelocity(newVel);
    }

    // Столкновение с блоками
    for (auto& brick : bricks_) {
        if (!brick->isDestroyed() && ball_->getGlobalBounds().intersects(brick->getBounds())) {
            brick->hit();

            if (brick->shouldBallBounce()) {
                // Определение стороны столкновения
                sf::FloatRect ballBounds = ball_->getGlobalBounds();
                sf::FloatRect brickBounds = brick->getBounds();

                float overlapLeft = ballBounds.left + ballBounds.width - brickBounds.left;
                float overlapRight = brickBounds.left + brickBounds.width - ballBounds.left;
                float overlapTop = ballBounds.top + ballBounds.height - brickBounds.top;
                float overlapBottom = brickBounds.top + brickBounds.height - ballBounds.top;

                bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
                bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
                bool fromTop = overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom;
                bool fromBottom = overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop;

                if (fromLeft || fromRight) ball_->reverseX();
                if (fromTop || fromBottom) ball_->reverseY();
            }

            break;
        }
    }
}

void GameState::checkGameConditions() {
    // Проверка проигрыша
    if (ball_->getPosition().y - ball_->getRadius() > 600) {
        gameLost_ = true;
        return;
    }

    // Проверка победы
    bool allDestroyed = true;
    for (const auto& brick : bricks_) {
        if (!brick->isDestroyed()) {
            allDestroyed = false;
            break;
        }
    }

    if (allDestroyed) {
        gameWon_ = true;
    }
}

void GameState::checkLoseCondition()
{
}

void GameState::render() {
    window_->clear(sf::Color::Black);

    // Отрисовка блоков
    for (const auto& brick : bricks_) {
        brick->draw(*window_);
    }

    // Отрисовка платформы и мяча
    window_->draw(platform_->getShape());
    window_->draw(ball_->getShape());

    // Отрисовка экранов победы/проигрыша
    if (gameWon_) window_->draw(winText_);
    if (gameLost_) window_->draw(loseText_);

    window_->display();
}

void GameState::showWinScreen() {
    window_->clear(sf::Color::Black);
    window_->draw(winText_);
    window_->display();
}

void GameState::handleWinScreenInput() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_->close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Y) {
                resetGame();
            }
            else if (event.key.code == sf::Keyboard::N) {
                window_->close();
            }
        }
    }
}

void GameState::showLoseScreen() {
    window_->clear(sf::Color::Black);
    window_->draw(loseText_);
    window_->display();
}

void GameState::handleLoseScreenInput() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_->close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Y) {
                resetGame();
            }
            else if (event.key.code == sf::Keyboard::N) {
                window_->close();
            }
        }
    }
}

void GameState::resetGame() {
    // Сброс состояния игры
    gameWon_ = false;
    gameLost_ = false;
    currentBallSpeedMultiplier_ = 1.0f;
    ballSpeedChangeTimer_ = 0.0f;

    // Пересоздание блоков
    initBricks();

    // Сброс позиции платформы
    platform_->setPosition(400 - 50, 600 - 50);

    // Сброс мяча
    ball_->reset(400, 500);
    ball_->setVelocity(sf::Vector2f(180.f, -220.f));
    ball_->setColor(sf::Color::Red);
}