#include "stdafx.hpp"
#include <SFML/Graphics.hpp> //SFML 2.5.1
#include "Map.hpp"
#include "Unit.hpp"
#include <iostream>

const sf::Time frameTime = sf::seconds(1.f / 20.f);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Algorithms");
    Map GameMap(window, 25, 25);
    Unit player(GameMap, false, 164);
    Unit enemy(GameMap, true, 27);
    bool testAStar = true;
    std::list<int> aStarPath;
    sf::CircleShape aStarDot;
    aStarDot.setRadius(5.f);
    aStarDot.setFillColor(sf::Color::Green);
    aStarDot.setOrigin({ 5.f - (GameMap.tileW / 2.f),5.f - (GameMap.tileH / 2.f) });
    sf::Clock dtClock;

    while (window.isOpen())
    {
        //Reset game loop values
        window.clear();
        GameMap.newClick = false;
        GameMap.endClick = false;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::P) {
                    //Test a new path by pressing P
                    testAStar = true;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left) {
                    //Register a new mouse click for moving units
                    GameMap.clickDown = true;
                    GameMap.newClick = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.key.code == sf::Mouse::Left) {
                    //Reset to unclicked
                    GameMap.clickDown = false;
                    GameMap.endClick = true;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                GameMap.mousePos = GameMap.getTileN(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            }
        }
        GameMap.drawMap(window);
        player.update(window);
        enemy.update(window);
        if (testAStar) {
            aStarPath = GameMap.astar(enemy.position, player.position);
            testAStar = false;
        }
        for (int tile : aStarPath) {
            aStarDot.setPosition(GameMap.getTilePos(tile));
            window.draw(aStarDot);
        }
        window.display();
    }

    return 0;
}
