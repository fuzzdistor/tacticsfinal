#ifndef TF_SCENE_HPP
#define TF_SCENE_HPP

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void onMousePressed(sf::Mouse::Button button, const sf::Vector2f& position);
    virtual void onMouseReleased(sf::Mouse::Button button, const sf::Vector2f& position);
    virtual void onMouseMoved(sf::Vector2f movement);
    virtual void onKeyPressed(sf::Keyboard::Key key);
    virtual void onKeyReleased(sf::Keyboard::Key key);
    virtual void update(sf::Time dt);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

    sf::View getView() const;
    sf::View& getView();
    void setView(const sf::View& view);

protected:
    friend void imguiWidget();

private:
    sf::View m_sceneView;
};

#endif //  TF_SCENE_HPP
