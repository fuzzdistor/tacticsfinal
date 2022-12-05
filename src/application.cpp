#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include "game.hpp"
#include "utils.hpp"

int main()
{
    TIMERINIT(ApplicationTimer);

    Game game;
    game.run();

    DBGTIMER(ApplicationTimer);
}
