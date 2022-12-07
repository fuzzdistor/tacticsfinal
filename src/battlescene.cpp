#include "battlescene.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>
#include "imgui.h"
#include "tweener.hpp"

void BattleScene::draw(sf::RenderTarget &target,
                       sf::RenderStates states) const 
{
    states.transform.scale({ 8.f, 8.f });
    target.setView(getView());
    target.draw(m_board, states);
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

BattleScene::BattleScene()
    : m_board(m)
    , m_tweener(Tweener::getInstance())
{
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
    ImGui::SliderFloat2("View center", viewcenter, 0.f, 300.f);
    scene->getView().setCenter(viewcenter[0], viewcenter[1]);
    ImGui::Text("View center is %2.2f, %2.2f", static_cast<double>(scene->getView().getCenter().x), static_cast<double>(scene->getView().getCenter().y));
    ImGui::Text("View size is %2.2f, %2.2f", static_cast<double>(scene->getView().getSize().x), static_cast<double>(scene->getView().getSize().y));
    ImGui::End();
}

