#include "World.h"

#include <iostream>
#include "Random.h"

World::World()
{
    for (uint32_t i = 0; i < m_Map.size(); i++)
    {
        if (Random::Value() > CREATURE_RATIO)
        {
            auto creature = std::make_shared<Creature>();
            creature->position = sf::Vector2u(i % WORLD_SIZE_X, i / WORLD_SIZE_X);

            m_Map[i] = creature;
            m_Creatures.push_back(creature);
        }
        else
        {
            m_Map[i] = nullptr;
        }
    }
}

void World::Render(sf::RenderTarget& target)
{
    // sf::RectangleShape shape(sf::Vector2f(CELL_RENDER_SIZE - CELL_PADDING_SIZE, CELL_RENDER_SIZE - CELL_PADDING_SIZE));
    sf::CircleShape shape((CELL_RENDER_SIZE - CELL_PADDING_SIZE) * 0.5f);

    for (uint32_t x = 0; x < WORLD_SIZE_X; x++)
    {
        for (uint32_t y = 0; y < WORLD_SIZE_Y; y++)
        {
            shape.setPosition(sf::Vector2f(x * CELL_RENDER_SIZE, y * CELL_RENDER_SIZE));
            shape.setFillColor(m_Map[x + y * WORLD_SIZE_X] ? m_Map[x + y * WORLD_SIZE_X]->brain.GetColor() : sf::Color::Black);
            target.draw(shape);
        }
    }
}

void World::Update()
{
    for (auto creature : m_Creatures)
    {
        auto out = creature->Update({ (float)creature->position.x, (float)creature->position.y, (float)creature->age });
        // for (int i = 0; i < out.size(); i++)
        //     std::cout << out[i] << " ";
        // std::cout << " ";

        if (out[Neuron::OUT0] > ACTIVATION_VALUE && creature->position.x < WORLD_SIZE_X - 1 && !m_Map[creature->position.x + 1 + creature->position.y * WORLD_SIZE_X])
        {
            m_Map[creature->position.x + 1 + creature->position.y * WORLD_SIZE_X] = creature;
            m_Map[creature->position.x + creature->position.y * WORLD_SIZE_X] = nullptr;

            creature->position.x += 1;
        }
        else if (out[Neuron::OUT0] < -ACTIVATION_VALUE && creature->position.x > 0 && !m_Map[creature->position.x - 1 + creature->position.y * WORLD_SIZE_X])
        {
            m_Map[creature->position.x - 1 + creature->position.y * WORLD_SIZE_X] = creature;
            m_Map[creature->position.x + creature->position.y * WORLD_SIZE_X] = nullptr;

            creature->position.x -= 1;
        }

        if (out[Neuron::OUT1] > ACTIVATION_VALUE && creature->position.y < WORLD_SIZE_Y - 1 && !m_Map[creature->position.x + (creature->position.y + 1) * WORLD_SIZE_X])
        {
            m_Map[creature->position.x + (creature->position.y + 1) * WORLD_SIZE_X] = creature;
            m_Map[creature->position.x + creature->position.y * WORLD_SIZE_X] = nullptr;

            creature->position.y += 1;
        }
        else if (out[Neuron::OUT1] < -ACTIVATION_VALUE && creature->position.y > 0 && !m_Map[creature->position.x + (creature->position.y - 1) * WORLD_SIZE_X])
        {
            m_Map[creature->position.x + (creature->position.y - 1) * WORLD_SIZE_X] = creature;
            m_Map[creature->position.x + creature->position.y * WORLD_SIZE_X] = nullptr;

            creature->position.y -= 1;
        }

        if (std::abs(out[Neuron::OUT2]) > ACTIVATION_VALUE)
        {
            std::array<sf::Vector2i, 8> emptyNeighbors;
            uint8_t emptyNeighborCount = 0;

            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    int newx = creature->position.x + i;
                    int newy = creature->position.y + j;

                    if (newx >= 0 && newy >= 0 && newx < WORLD_SIZE_X && newy < WORLD_SIZE_Y && !m_Map[newx + newy * WORLD_SIZE_X])
                    {
                        emptyNeighbors[emptyNeighborCount] = sf::Vector2i(i, j);
                        emptyNeighborCount++;
                    }
                }
            }

            if (emptyNeighborCount > 0)
            {
                auto offset = emptyNeighbors[(int)(Random::Value() * emptyNeighborCount)];

                m_Map[creature->position.x + offset.x + (creature->position.y + offset.y) * WORLD_SIZE_X] = creature;
                m_Map[creature->position.x + creature->position.y * WORLD_SIZE_X] = nullptr;

                creature->position.x += offset.x;
                creature->position.y += offset.y;
            }
        }
    }
}

void World::Reproduce()
{
    std::vector<std::shared_ptr<Creature>> survivors;
    survivors.reserve(m_Creatures.size());

    for (auto creature : m_Creatures)
    {
        // if (creature->position.x > WORLD_SIZE_X * 0.8 || creature->position.x < WORLD_SIZE_X * 0.2)
        if (creature->position.y < WORLD_SIZE_Y * 0.3)
        // constexpr float tresh = 0.2;
        // if (creature->position.x > WORLD_SIZE_X * tresh && creature->position.x < WORLD_SIZE_X * (1 - tresh) && creature->position.y > WORLD_SIZE_Y * tresh && creature->position.y < WORLD_SIZE_Y * (1 - tresh))
        {
            survivors.push_back(creature);
        }
    }

    std::cout << "Survival Rate: " << (float)survivors.size() / m_Creatures.size() * 100 << std::endl;

    if (survivors.empty())
        return;

    m_Creatures.clear();

    for (uint32_t i = 0; i < m_Map.size(); i++)
    {
        if (Random::Value() > CREATURE_RATIO)
        {
            auto parent1 = survivors[Random::Value() * survivors.size()];
            auto parent2 = survivors[Random::Value() * survivors.size()];

            auto creature = std::make_shared<Creature>(parent1, parent2);
            creature->position = sf::Vector2u(i % WORLD_SIZE_X, i / WORLD_SIZE_X);

            m_Map[i] = creature;
            m_Creatures.push_back(creature);
        }
        else
        {
            m_Map[i] = nullptr;
        }
    }
}
