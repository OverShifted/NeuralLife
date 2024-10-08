#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Config.h"
#include "World.h"
#include "Random.h"

#include <iostream>

int main()
{
    // Random::Seed();
    World world;
    uint32_t step = 0;
    uint32_t generation = 0;

    sf::RenderWindow window(sf::VideoMode(WORLD_SIZE_X * CELL_RENDER_SIZE, WORLD_SIZE_Y * CELL_RENDER_SIZE), "NeuralLife: Generation #0");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (step == GENERATION_LIFETIME)
        {
            step = 0;
            generation++;
            world.Reproduce();

            window.setTitle("NeuralLife: Generation #" + std::to_string(generation));
        }

        world.Update();

        window.clear();
        world.Render(window);
        window.display();

        // sf::sleep(sf::seconds(0.01));

        step++;
    }

    return 0;
}
