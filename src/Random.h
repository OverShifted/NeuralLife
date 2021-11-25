#include <random>

class Random
{
public:
    static void Seed() { s_Generator.seed(std::random_device()()); }
    static float Value() { return (float)s_Distribution(s_Generator) / UINT32_MAX; }

private:
    static std::default_random_engine s_Generator;
    static std::uniform_int_distribution<uint32_t> s_Distribution;
};