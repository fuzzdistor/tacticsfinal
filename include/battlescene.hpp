#include "SFML/Graphics/View.hpp"
#include "scene.hpp"
#include "board.hpp"
#include "maps.data.hpp"

class BattleScene : public Scene
{
public:
    BattleScene();
    void onMousePressed(sf::Mouse::Button button, const sf::Vector2f& mousePosition) final;
    void onMouseMoved(sf::Vector2f movement) final;
    void onKeyPressed(sf::Keyboard::Key key) final;
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const final;

private:
    Board m_board;
    mutable sf::View m_sceneView;
};

