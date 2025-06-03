#include <memory>
#include "GameState.h"

class Application 
{
public:
    Application();
    ~Application();
    void run();

private:
    std::unique_ptr<GameState> gameState_;
};