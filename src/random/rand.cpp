#include "rand.hpp"
#include <random>

double rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

bool pileFace(double p) // TODO : à renommer
{
    // constexpr float epsilon = 0.5;
    // assert(p > 0. && p < 1. && "p not in the good interval");
    return rand01() < p;
}

float loiExp(float lambda)
{
    float u = rand01();
    return -1. / lambda * std::log(1. - u); // = Z
}

double genDirection()
{
    double speed = loiExp(100.) + 0.5;
    return pileFace(0.5) ? speed : -1 * speed;
}
