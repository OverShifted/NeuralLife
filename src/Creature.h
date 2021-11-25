#include <array>

#include <SFML/Graphics.hpp>
#include "Config.h"

struct Neuron
{
    enum ID : uint16_t
    {
        INPUT0 = 0,
        INPUT1 = 1,
        INPUT2 = 2,

        OUT0 = 0,
        OUT1 = 1,
        OUT2 = 2,

        HIDDEN_NESS = (1 << 15),
        HIDDEN0 = HIDDEN_NESS | 0,
        HIDDEN1 = HIDDEN_NESS | 1,
    };

    bool IsHidden() const { return (1 << 15) & id; }
    uint16_t GetIndex() const { return (~(1 << 15)) & id; }

    uint16_t id;
};

struct Connection
{
    Neuron src;
    Neuron dest;
    float weight;
};

struct Brain
{
    Brain();
    void Randomize();

    using InputData = std::array<float, INPUT_NEURON_COUNT>;
    using OutputData = std::array<float, OUTPUT_NEURON_COUNT>;

    OutputData Update(const InputData& input);

    sf::Color GetColor();

    std::array<Connection, CONNECTION_COUNT> connections;
    std::array<float, HIDDEN_NEURON_COUNT> hiddenNeurons;
};

struct Creature
{
    Creature() = default;
    Creature(Creature* parent1, Creature* parent2);

    Brain::OutputData Update(Brain::InputData input) 
    {
        age++;
        return brain.Update(input);
    }

    Brain brain;
    sf::Vector2u position;
    uint32_t age = 0;
};