#include "Random.h"

std::default_random_engine Random::s_Generator;
std::uniform_int_distribution<uint32_t> Random::s_Distribution(0, UINT32_MAX);
