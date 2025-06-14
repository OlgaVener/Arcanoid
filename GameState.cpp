#include "GameState.h"
#include <string>

GameState::GameState(unsigned int width, unsigned int height) :
    window_(std::make_unique<sf::RenderWindow>(
        sf::VideoMode(800, 600),  // Фиксированное разрешение
        "Arkanoid",
        sf::Style::Titlebar | sf::Style::Close)),  // Закрывающая скобка для make_unique
    platform_(std::make_unique<Platform>(width / 2 - 50, height - 50, 100, 20)),
    ball_(std::make_unique<Ball>(width / 2, height - 100, 10.f))
{
    window_->setFramerateLimit(60);

    // Начальная скорость
    sf::Vector2f initialVelocity(180.f, -220.f);
    float length = sqrt(initialVelocity.x * initialVelocity.x + initialVelocity.y * initialVelocity.y);
    if (length > 0) {
        initialVelocity = (initialVelocity / length) * 280.f;
    }
    ball_->setVelocity(initialVelocity);

    if (!font_.loadFromFile("arial.ttf")) {
        // Обработка ошибки
    }

    loseText_.setFont(font_);
    loseText_.setCharacterSize(40);
    loseText_.setFillColor(sf::Color::White);
    loseText_.setString(
        "Game Over!\n"
        "Try again?\n"
        "(Y - Yes, N - No)"
    );
    sf::FloatRect loseBounds = loseText_.getLocalBounds();
    loseText_.setOrigin(loseBounds.width / 2, loseBounds.height / 2);
    loseText_.setPosition(400, 300);

    winText_.setFont(font_);
    winText_.setCharacterSize(40);
    winText_.setFillColor(sf::Color::White);
    winText_.setString(
        "Congratulations! You won!\n"
        "Play again?\n"
        "(Y - Yes, N - No)"
    );
    sf::FloatRect winBounds = winText_.getLocalBounds();
    winText_.setOrigin(winBounds.width / 2, winBounds.height / 2);
    winText_.setPosition(400, 300);

    initBricks();
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
    checkLoseCondition();
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

        // Увеличиваем множитель скорости
        currentBallSpeedMultiplier_ = std::min(
            maxBallSpeedMultiplier_,
            currentBallSpeedMultiplier_ * 1.05f
        );

        // Визуальная индикация ускорения
        float speedRatio = (currentBallSpeedMultiplier_ - minBallSpeedMultiplier_) /
            (maxBallSpeedMultiplier_ - minBallSpeedMultiplier_);
        int redValue = 100 + static_cast<int>(155 * speedRatio);
        const_cast<sf::CircleShape&>(ball_->getShape()).setFillColor(sf::Color(redValue, 100, 100));

        ball_->setSpeedMultiplier(currentBallSpeedMultiplier_);
    }
}

void Brick::setColor(const sf::Color& color) {
    shape_.setFillColor(color);
}

void Brick::setOutline(float thickness, const sf::Color& color) {
    shape_.setOutlineThickness(thickness);
    shape_.setOutlineColor(color);
}

void GameState::initBricks() {
    bricks_.clear();

    const int rows = 5;
    const int cols = 8; // Оптимальное количество столбцов
    const float brickWidth = 90.f; // Ширина блока
    const float brickHeight = 25.f; // Высота блока
    const float paddingX = 8.f; // Горизонтальный отступ
    const float paddingY = 5.f; // Вертикальный отступ

    // Центрирование блоков
    float totalWidth = cols * brickWidth + (cols - 1) * paddingX;
    float startX = (800.f - totalWidth) / 2.f;
    const float startY = 30.f;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int hitPoints = 1;
            if (row == 0) hitPoints = 3;
            else if (row < 2) hitPoints = 2;

            bricks_.push_back(std::make_unique<Brick>(
                startX + col * (brickWidth + paddingX),
                startY + row * (brickHeight + paddingY),
                hitPoints
            ));
        }
    }
}

void GameState::checkCollisions() {
    // Границы экрана
    float ballLeft = ball_->getPosition().x - ball_->getRadius();
    float ballRight = ball_->getPosition().x + ball_->getRadius();
    float ballTop = ball_->getPosition().y - ball_->getRadius();
    float ballBottom = ball_->getPosition().y + ball_->getRadius();

    // Столкновение со стенами
    if (ballLeft <= 0 || ballRight >= 800) {
        ball_->reverseX();
    }
    if (ballTop <= 0) {
        ball_->reverseY();
    }

    // Столкновение с платформой
    sf::FloatRect platformBounds = platform_->getGlobalBounds();
    if (ballBottom >= platformBounds.top &&
        ball_->getPosition().x >= platformBounds.left &&
        ball_->getPosition().x <= platformBounds.left + platformBounds.width) {

        // Рассчитываем точку удара (-1..1)
        float hitPos = (ball_->getPosition().x - platformBounds.left) / platformBounds.width * 2 - 1;

        // Меняем направление с учетом точки удара
        sf::Vector2f newVel;
        newVel.x = hitPos * 300.f;
        newVel.y = -sqrt(300.f * 300.f - newVel.x * newVel.x);
        ball_->setVelocity(newVel);
    }

    // Столкновение с блоками
    for (auto& brick : bricks_) {
        if (!brick->isDestroyed() && ball_->getGlobalBounds().intersects(brick->getBounds())) {
            brick->hit();
            ball_->reverseY();
            break;
        }
    }
}

void GameState::checkLoseCondition() {
    if (ball_->getPosition().y - ball_->getRadius() > 600) {  // Мяч упал ниже экрана
        gameLost_ = true;
    }
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
        // Центрируем текст
        sf::FloatRect textBounds = winText_.getLocalBounds();
        winText_.setOrigin(textBounds.width / 2, textBounds.height / 2);
        winText_.setPosition(window_->getSize().x / 2, window_->getSize().y / 2);
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
                // Возврат в меню или закрытие
                window_->close();
            }
        }
    }
}

void GameState::resetGame() {
    // Полный сброс игры
    initBricks();

    // Сброс мяча
    ball_->reset(400, 300);  // Центр экрана
    ball_->setVelocity(sf::Vector2f(180.f, -220.f));

    // Сброс платформы
    platform_->setPosition(350, 550);  // 800/2 - 50, 600-50

    // Сброс состояний
    gameWon_ = false;
    gameLost_ = false;
    currentBallSpeedMultiplier_ = 1.0f;
    ballSpeedChangeTimer_ = 0.0f;
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