#include "battlescene.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>

void BattleScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    target.setView(m_sceneView);
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
    movement = m_sceneView.getTransform().transformPoint(movement);
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
    : m_board( m )
    , m_sceneView({46, 32}, {100, 100})
{
}
