#include <array>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Config.h"
#include "Creature.h"

class World 
{
public:
    World();

    void Render(sf::RenderTarget& target);
    void Update();

    void Reproduce();

private:
    std::array<std::shared_ptr<Creature>, WORLD_SIZE_X * WORLD_SIZE_Y> m_Map;
    std::vector<std::shared_ptr<Creature>> m_Creatures;
};