#include "scene.hpp"

void Scene::onMousePressed(sf::Mouse::Button, const sf::Vector2f&){}
void Scene::onMouseReleased(sf::Mouse::Button, const sf::Vector2f&){}
void Scene::onMouseMoved(sf::Vector2f){}
void Scene::onKeyPressed(sf::Keyboard::Key){}
void Scene::onKeyReleased(sf::Keyboard::Key){}
void Scene::update(sf::Time){}

void Scene::setResetCallback(std::function<void(void)> callback)
{
    m_resetScene = callback;
}

void Scene::setQuitCallback(std::function<void(void)> callback)
{
    m_quitProgram = callback;
}

void Scene::resetScene()
{
    m_resetScene();
}

void Scene::quit()
{
    m_quitProgram();
}

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

