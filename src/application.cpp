#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "map.hpp"
#include "utils.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <cassert>
#include <span>
#include <string_view>

char m[] =
    "wwww......." \
    "..x........" \
    "..xx......." \
    "..........." \
    "..........." \
    "....--.....";


int main()
{
    Map map(m, 11, 6);

    sf::RenderWindow window({300, 300}, "SFML WORKS");
    const sf::Font font = [](){
        sf::Font _font;
        _font.loadFromFile("media/fonts/default.ttf");
        return _font;
        }();

    sf::Vector2u position = {3, 3};


    std::vector<sf::Text> t;
    for (uint i = 0; i < 11 ; i++)
        for (uint j = 0; j < 6 ; j++)
        {
            t.emplace_back();
            t.back().setFont(font);
            t.back().setFillColor(sf::Color::White);
            t.back().setCharacterSize(20);
            t.back().setPosition(sf::Vector2f(i*20u,j*20u));
            if(masksMatch(map.getTerrain(i,j), Map::Terrain::Walkable))
                t.back().setString("x");
        }


    std::vector<sf::Text> textvec;

    auto settextvec = [&](){
        textvec.clear();
        auto vec = map.getRangeRadius(position, 3);
        for (const auto& pos : vec)
        {
            textvec.emplace_back();
            auto& text = textvec.back();
            text.setFont(font);
            text.setCharacterSize(20);
            if (pos == position)
                text.setFillColor(sf::Color::Cyan);
            else
                text.setFillColor(sf::Color::Green);
            text.setPosition(sf::Vector2f(pos*20u));
            text.setString("x");
        }
    };

    settextvec();

    sf::Clock clock;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (clock.getElapsedTime() > sf::seconds(0.3f))
                {
                    if (event.key.code == sf::Keyboard::W)
                    {
                        position.y--;
                        settextvec();
                        clock.restart();
                    }
                    if (event.key.code == sf::Keyboard::A)
                    {
                        position.x--;
                        settextvec();
                        clock.restart();
                    }
                    if (event.key.code == sf::Keyboard::S)
                    {
                        position.y++;
                        settextvec();
                        clock.restart();
                    }
                    if (event.key.code == sf::Keyboard::D)
                    {
                        position.x++;
                        settextvec();
                        clock.restart();
                    }
                }
            }
        }

        window.clear();
        for (const auto& text: t)
            window.draw(text);
        for (const auto& text: textvec)
            window.draw(text);
        window.display();
    }
}
