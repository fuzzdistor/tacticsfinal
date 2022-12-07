#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include "battlescene.hpp"
#include "game.hpp"
#include "resourcemanager.hpp"
#include <memory>

#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
    : currentScene()
    , m_frameTimer()
{
    init();
    currentScene = std::make_unique<BattleScene>();
    currentScene->setResetCallback([&]{ resetScene(); });
}

void Game::init()
{
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    auto& pack = ResourcePack::getInstance();
    pack.textures.load(fd::hash("Allied"), "media/textures/unit1.png");
    pack.textures.load(fd::hash("Enemy"), "media/textures/unit2.png");
    pack.textures.load(fd::hash("Cursor"), "media/textures/cursor.png");
    pack.fonts.load(fd::hash("Default"), "media/fonts/default.ttf");
    defaultView = window.getView();
}

void Game::update()
{
    sf::Time frameTime = m_frameTimer.restart();
    ImGui::SFML::Update(window, frameTime);
    currentScene->update(frameTime);
}

void Game::resetScene()
{
    currentScene = std::make_unique<BattleScene>();
}

void Game::run()
{
    m_frameTimer.restart();
    while(window.isOpen())
    {
        handleEvents();
        update();
        draw();
    }
    ImGui::SFML::Shutdown();
}

void Game::draw()
{
    window.clear(sf::Color(BGCOLOR));
    window.setView(defaultView);
    currentScene->draw(window, sf::RenderStates());
    ImGui::SFML::Render(window);
    window.display();
}

void Game::handleEvents()
{

    sf::Event event = sf::Event();
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
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
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return;
                }
                if (event.key.code == sf::Keyboard::F11)
                {
                    if (isFullscreen)
                        window.create(VIDEOMODE, WINDOW_TITLE);
                    else
                        window.create(sf::VideoMode::getDesktopMode(), WINDOW_TITLE, sf::Style::Fullscreen);

                    isFullscreen = !isFullscreen;
                }
                break;
            case sf::Event::LostFocus:
                {
                    window.setFramerateLimit(10);
                }
                break;
            case sf::Event::GainedFocus:
                {
                    window.setFramerateLimit(60);
                }
                break;
            case sf::Event::Resized:
                onWindowResized(event.size.width, event.size.height);
                break;
            default: ;
        }
    }
}

void Game::onWindowResized(unsigned int width, unsigned int height)
{
    defaultView.setSize({static_cast<float>(width), static_cast<float>(height)});
    defaultView.setCenter(defaultView.getSize()/3.f);
    currentScene->setView(defaultView);
}

