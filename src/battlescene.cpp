#include "battlescene.hpp"
#include "SFML/Config.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>
#include "imgui.h"
#include "resourcemanager.hpp"
#include "tween.hpp"
#include "tweener.hpp"
#include "utils.hpp"

BattleScene::BattleScene()
    : m_board(m)
    , m_screen()
    , m_startText()
    , m_tweener(Tweener::getInstance())
{
    setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)));
    m_startText.setFont(ResourcePack::getInstance().fonts.get(fd::hash("Default")));
    m_startText.setCharacterSize(80);
    m_startText.setFillColor(sf::Color::White);
    m_startText.setString("BATTLE START!");
    m_startText.setPosition(getView().getSize() / 2.f - m_startText.getLocalBounds().getSize() / 2.f);
    m_screen.setSize(getView().getSize());
    m_screen.setFillColor(sf::Color(0x000000AA));
    m_tweener.createTween(
            [timer = sf::Time::Zero, tween = Tween<uint>(), alpha = 255u, twinkle = 0u, this](sf::Time dt) mutable
            {
            if (!tween.isActive())
            {
                tween.setTween(&alpha, 255u, 0u, sf::seconds(1.5f));
                tween.setTween(&alpha, 255u, 0u, sf::seconds(1.5f));
                m_screen.setFillColor(sf::Color(0, 0, 0, sf::Uint8(alpha)));
                m_startText.setFillColor(sf::Color(255, 255, 255, sf::Uint8(alpha)));
            }

            twinkle = std::lerp(0, 10, timer.asSeconds()/5.f);
            m_startText.setFillColor(sf::Color(255, 255, 255, sf::Uint8(alpha * (twinkle%2))));

            if (timer > sf::seconds(2.f))
            {
            m_screen.setFillColor(sf::Color(0, 0, 0, sf::Uint8(alpha)));
            tween.update(dt);
            }
            timer += dt;

            return !tween.isActive();
            });
}

void BattleScene::draw(sf::RenderTarget &target,
                       sf::RenderStates states) const
{
    states.transform.scale({ 8.f, 8.f });
    target.setView(getView());
    target.draw(m_board, states);
    target.setView(target.getDefaultView());
    target.draw(m_screen);
    target.draw(m_startText);

}

void BattleScene::onMousePressed(sf::Mouse::Button button, const sf::Vector2f&)
{
    if (button == sf::Mouse::Button::Left)
    {
        m_board.accept();
    }
}

void BattleScene::onMouseMoved(sf::Vector2f movement)
{
    movement = getView().getTransform().transformPoint(movement);
    movement.y = -movement.y;
    m_board.setCursorPosition(sf::Vector2u(movement));
}

void BattleScene::onKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::W:
            m_board.moveCursor(-sf::Vector2u(0, 1));
            break;
        case sf::Keyboard::S:
            m_board.moveCursor({0, 1});
            break;
        case sf::Keyboard::A:
            m_board.moveCursor(-sf::Vector2u(1, 0));
            break;
        case sf::Keyboard::D:
            m_board.moveCursor({1, 0});
            break;
        case sf::Keyboard::J:
            m_board.accept();
            break;
        case sf::Keyboard::K:
            m_board.cancel();
            break;
        default:;
    }
}

void BattleScene::update(sf::Time dt)
{
    m_board.update(dt);
    m_tweener.update(dt);
    imguiWidget(this);
}

void imguiWidget(Scene* scene)
{
    auto battlescene = static_cast<BattleScene*>(scene);
    // ImGui::ShowDemoWindow();
    ImGui::Begin("TurnManager");
    auto highestCtUnit = battlescene->m_board.m_turnManager.getHighestCtUnit();
    ImGui::End();
    ImGui::Begin("BattleScene");
    static float viewcenter[2] = { scene->getView().getCenter().x, scene->getView().getCenter().y };
    ImGui::SliderFloat2("View center", viewcenter, 0.f, 3000.f);
    scene->getView().setCenter(viewcenter[0], viewcenter[1]);
    ImGui::Text("View center is %2.2f, %2.2f", static_cast<double>(scene->getView().getCenter().x), static_cast<double>(scene->getView().getCenter().y));
    ImGui::Text("View size is %2.2f, %2.2f", static_cast<double>(scene->getView().getSize().x), static_cast<double>(scene->getView().getSize().y));
    ImGui::End();
}

