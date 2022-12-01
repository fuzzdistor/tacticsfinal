#include "game.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "resourcemanager.hpp"
#include "utils.hpp"
#include "battlescene.hpp"
#include <memory>

Game::Game()
    : currentScene()
{
    init();
    currentScene = std::make_unique<BattleScene>();
}

void Game::init()
{
    auto& pack = ResourcePack::getInstance();
    pack.textures.load("Allied", "media/textures/unit1.png");
    pack.textures.load("Enemy", "media/textures/unit2.png");
    pack.textures.load("Cursor", "media/textures/cursor.png");
}

void Game::run()
{
    while(window.isOpen())
    {
        handleEvents();
        draw();
    }
}

void Game::draw()
{
    window.clear(sf::Color(BGCOLOR));
    window.setView(defaultView);
    currentScene->draw(window);
    window.display();
}

void Game::handleEvents()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        window.close();
        return;
    }

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseMoved:
                currentScene->onMouseMoved(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                break;
            case sf::Event::MouseButtonPressed:
                currentScene->onMousePressed(event.mouseButton.button, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                break;
            case sf::Event::MouseButtonReleased:
                currentScene->onMouseReleased(event.mouseButton.button, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                break;
            case sf::Event::KeyReleased:
                currentScene->onKeyReleased(event.key.code);
                break;
            case sf::Event::KeyPressed:
                currentScene->onKeyPressed(event.key.code);
                break;
            case sf::Event::LostFocus:
                // TODO
                break;
            case sf::Event::GainedFocus:
                // TODO
                break;
            case sf::Event::Resized:
                // TODO
                break;
            default: ;
        }
    }
}

