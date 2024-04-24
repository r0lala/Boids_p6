#pragma once
#include "flower.hpp"

void Flower::moveFlower(p6::Context& ctx, float wall)
{
    if (ctx.key_is_pressed(GLEW_KEY_Z))
    {
        this->moveFront(ctx.delta_time(), wall);
    }
    if (ctx.key_is_pressed(GLEW_KEY_S))
    {
        this->moveBack(-ctx.delta_time(), wall);
    }
    if (ctx.key_is_pressed(GLEW_KEY_Q))
    {
        this->moveLeft(ctx.delta_time(), wall);
    }
    if (ctx.key_is_pressed(GLEW_KEY_D))
    {
        this->moveLeft(-ctx.delta_time(), wall);
    }
}

void Flower::moveFront(float delta, float wall)
{
}
void Flower::moveBack(float delta, float wall)
{
}
void Flower::moveLeft(float delta, float wall)
{
}
void Flower::moveRight(float delta, float wall)
{
}