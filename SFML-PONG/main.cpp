#include "Include/GameplayLoop.h"
#include "Include/GameStates/GameplayState.h"

int main()
{

    GameplayLoop game;

    // Start the game directly with the GameplayState
    game.setState(new GameplayState());
    game.initialize();  //init resources (if any)
    game.run();  // Run the game loop
    return 0;

}
