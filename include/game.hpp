#ifndef TF_GAME_HPP
#define TF_GAME_HPP

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "scene.hpp"

#ifndef NDEBUG
#define WINDOW_TITLE "Tactics Final - DEBUG"
#else
#define WINDOW_TITLE "Tactics Final"
#endif

#define VIDEOMODE sf::VideoMode(1280, 720)
constexpr sf::Uint32 BGCOLOR = 0x37284fff;

class Game
{
public:
    Game();
    void run();

private:
    void init();
    void handleEvents();
    void draw();
    void update();
    void onWindowResized(unsigned int width, unsigned int height);
    void resetScene();

    std::unique_ptr<Scene> currentScene;
    sf::RenderWindow window { VIDEOMODE, WINDOW_TITLE };
    sf::View defaultView;
    sf::Clock m_frameTimer;
    bool isFullscreen { false };
};

#endif // TF_GAME_HPP
