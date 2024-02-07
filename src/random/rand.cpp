#include "rand.hpp"
#include <random>

double rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

bool pileFace(double p)
{
    // constexpr float epsilon = 0.5;
    // assert(p > 0. && p < 1. && "p n'est pas dans la bonne intervalle");
    return rand01() < p;
}