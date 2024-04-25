#pragma once
#include <imgui.h>
#include "p6/p6.h"

class Options {
public: // TODO à mettre en pv
    // TODO regrouper var et coeff ?
    float align           = 0.3f;
    float coeffAlignement = 0.001f;

    float separate       = 0.1f;
    float coeffRepulsion = 1.f;

    float cohesion      = 0.6f;
    float coeffCohesion = 1.f;

    int lods = 1;

public:
    Options(p6::Context& ctx)
    {
        ctx.imgui = [&]() {
            ImGui::Begin("Parameters");
            ImGui::SliderInt("LODS", &lods, 0, 1);

            ImGui::SliderFloat("Alignment", &align, 0.f, 1.f);
            ImGui::SliderFloat("Separation", &separate, 1.f, 2.f);
            ImGui::SliderFloat("Cohesion", &cohesion, 0.f, 100.f);

            ImGui::SliderFloat("Coefficient d'alignement", &coeffAlignement, 0.f, 50.f);
            ImGui::SliderFloat("Coefficient de repulsion", &coeffRepulsion, 0.f, 50.f);
            ImGui::SliderFloat("Coefficient de cohesion", &coeffCohesion, 0.f, 50.f);
            ImGui::End();
        };
    }

    int getLods() const { return lods; }
};