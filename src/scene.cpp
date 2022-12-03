#include "scene.hpp"

void Scene::onMousePressed(sf::Mouse::Button, const sf::Vector2f&){}
void Scene::onMouseReleased(sf::Mouse::Button, const sf::Vector2f&){}
void Scene::onMouseMoved(sf::Vector2f){}
void Scene::onKeyPressed(sf::Keyboard::Key){}
void Scene::onKeyReleased(sf::Keyboard::Key){}
void Scene::update(sf::Time){}

sf::View& Scene::getView()
{
    return m_sceneView;
}

sf::View Scene::getView() const
{
    return m_sceneView;
}

void Scene::setView(const sf::View& view)
{
    m_sceneView = view;
}

