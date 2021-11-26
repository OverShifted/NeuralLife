#include "Creature.h"
#include "Random.h"

#include <iostream>

Brain::Brain()
{
    for (auto& connection : connections)
    {
        connection = Connection{
            Neuron{Neuron::INPUT0},
            Neuron{Neuron::OUT0},
            0.0f
        };
    }

    for (auto& neuron : hiddenNeurons)
    {
        neuron = 0.0f;
    }

    Randomize();

    // connections[0] = Connection{
    //     Neuron{Neuron::INPUT0},
    //     Neuron{Neuron::OUT0},
    //     -1.0f / 32.0f
    // };
}

static Neuron RandomNeuron(uint32_t nonHiddenCount)
{
    if (Random::Value() > 0.5)
        return Neuron{(uint16_t) (Neuron::HIDDEN_NESS | (uint16_t) (Random::Value() * HIDDEN_NEURON_COUNT))};

    return Neuron{(uint16_t) (Random::Value() * nonHiddenCount)};
}

static Connection RandomConnection()
{
    return Connection{
        RandomNeuron(INPUT_NEURON_COUNT),
        RandomNeuron(OUTPUT_NEURON_COUNT),
        (Random::Value() - 0.5f) * 2.0f
    };
}

void Brain::Randomize()
{
    for (auto& connection : connections)
    {
        connection = RandomConnection();
    }
}

Brain::OutputData Brain::Update(const Brain::InputData& input)
{
    std::array<float, HIDDEN_NEURON_COUNT> newInternalState;

    for (auto& neuron : newInternalState)
        neuron = 0.0f;

    Brain::OutputData output;

    for (auto& neuron : output)
        neuron = 0.0f;

    for (const auto& connection : connections)
    {
        float src = connection.src.IsHidden() ? hiddenNeurons[connection.src.GetIndex()] : input[connection.src.GetIndex()];
        float& dest = (connection.dest.IsHidden() ? newInternalState[connection.dest.GetIndex()] : output[connection.dest.GetIndex()]);
        dest += src * connection.weight;
    }

    hiddenNeurons = newInternalState;
    return output;
}

sf::Color Brain::GetColor()
{
    uint8_t r = ((int) connections.front().src.IsHidden())
              | ((int) connections.front().dest.IsHidden() << 1)
              | ((int) connections.front().src.GetIndex()  << 2)
              | ((int) connections.front().dest.GetIndex() << 3)

              | ((int) connections.back().src.IsHidden()   << 4)
              | ((int) connections.back().dest.IsHidden()  << 5)
              | ((int) connections.back().src.GetIndex()   << 6)
              | ((int) connections.back().dest.GetIndex()  << 7);

    uint8_t g = ((r & 0x1f) << 3);
    uint8_t b = ((r & 7)    << 5);

    return sf::Color(r, g, b);
}

Creature::Creature(std::shared_ptr<Creature> parent1, std::shared_ptr<Creature> parent2)
{
    for (uint32_t i = 0; i < brain.connections.size(); i++)
    {
        if (Random::Value() < MUTATION_RATE)
            brain.connections[i] = RandomConnection();
        else
            brain.connections[i] = (Random::Value() > 0.5 ? parent1 : parent2)->brain.connections[i];
    }
}
