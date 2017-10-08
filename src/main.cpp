#include "JTime.h"

#include <SFML/Graphics.hpp>

#include <iostream>


void process_events(sf::Window& _window)
{
    sf::Event event;
    while (_window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            _window.close();

        // Escape key: exit
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            _window.close();
    }
}


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defence Heatmap Pathfinding");

    JTime time;

    sf::Font font;
    font.loadFromFile("../arial.ttf");

    sf::Text delta_time_display;
    delta_time_display.setFont(font);
    delta_time_display.setCharacterSize(16);

    sf::Text time_display;
    time_display.setFont(font);
    time_display.setCharacterSize(16);
    time_display.setPosition(0, 16);

    // Start the game loop
    while (window.isOpen())
    {
        sf::sleep(sf::milliseconds(1));

        time.tick();
        window.clear();

        process_events(window);

        delta_time_display.setString("Delta Time: " + std::to_string(JTime::get_unscaled_delta_time()));
        time_display.setString("Time: " + std::to_string(JTime::get_unscaled_time()));

        window.draw(delta_time_display);
        window.draw(time_display);

        window.display();
    }

    return EXIT_SUCCESS;
}
