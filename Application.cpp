#include "Application.h"

Application::Application()
    : gameState_(std::make_unique<GameState>(800, 600)) {}

Application::~Application() = default;

void Application::run() 
{
    gameState_->run();
}