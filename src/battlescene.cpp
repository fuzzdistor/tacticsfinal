#include "battlescene.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>
#include "imgui.h"

void BattleScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

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
{
}

void BattleScene::update(sf::Time delta)
{
    imguiWidget(this);
}

void imguiWidget(BattleScene* scene)
{
    ImGui::ShowDemoWindow();
    ImGui::Begin("BattleScene");
    static int pos[2]; ImGui::InputInt2("Position", pos, ImGuiInputTextFlags_CharsDecimal);
    if(ImGui::Button("Move to position"))
        scene->m_board.moveCharacter({ static_cast<unsigned int>(pos[0]), static_cast<unsigned int>(pos[1]) });
    static float viewcenter[2] = { scene->getView().getCenter().x, scene->getView().getCenter().y };
    ImGui::SliderFloat2("View center", viewcenter, 0.f, 300.f);
    scene->getView().setCenter(viewcenter[0], viewcenter[1]);
    ImGui::Text("View center is %2.2f, %2.2f", scene->getView().getCenter().x, scene->getView().getCenter().y);
    ImGui::End();
}

