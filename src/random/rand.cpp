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

float loiBeta(float alpha, float beta)
{
    float v = rand01();
    float w = rand01();

    // Distribution b^ta
    float x = std::pow(v, 1.0f / alpha);
    float y = std::pow(w, 1.0f / beta);

    return x / (x + y);
}

float loiCauchy(float alpha, float x)
{
    float p = rand01();
    return x + alpha * std::tan(M_PI * (p - 0.5));
}

int loiBinomiale(int tries, float p)
{
    int x = 0;
    for (int i = 0; i < tries; ++i)
    {
        if (rand01() < p)
        {
            x++;
        }
    }
    return x;
}