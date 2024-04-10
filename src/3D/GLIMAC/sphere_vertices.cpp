#include "sphere_vertices.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

namespace glimac {

std::vector<ShapeVertex> sphere_vertices(float radius, size_t discLat, size_t discLong) // NOLINT(bugprone-easily-swappable-parameters, readability-inconsistent-declaration-parameter-name)
{
    // Équation paramétrique en (r, phi, theta) de la sphère
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= phi <= 2PI
    //
    // x(r, phi, theta) = r * sin(phi) * cos(theta)
    // y(r, phi, theta) = r * sin(theta) *
    // z(r, phi, theta) = r * cos(phi) * cos(theta)
    //
    // Discrétisation:
    // phi = 2PI / discLat, theta = PI / discLong
    //
    // x(r, i, j) = r * sin(i * phi) * cos(-PI / 2 + j * theta)
    // y(r, i, j) = r * sin(-PI / 2 + j * theta)
    // z(r, i, j) = r * cos(i * phi) * cos(-PI / 2 + j * theta)
    //
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs

    const auto fDiscLat  = static_cast<float>(discLat);
    const auto fDiscLong = static_cast<float>(discLong);

    const float phi   = 2.f * glm::pi<float>() / fDiscLat;
    const float theta = glm::pi<float>() / fDiscLong;

    std::vector<ShapeVertex> data;

    // Construit l'ensemble des vertex
    for (size_t j = 0; j <= discLong; ++j)
    {
        const auto fj = static_cast<float>(j);

        const float cosTheta = std::cos(-glm::pi<float>() / 2.f + fj * theta);
        const float sinTheta = std::sin(-glm::pi<float>() / 2.f + fj * theta);

        for (size_t i = 0; i <= discLat; ++i)
        {
            const auto  fi = static_cast<float>(i);
            ShapeVertex vertex{};

            vertex.texCoords.x = fi / fDiscLat;
            vertex.texCoords.y = 1.f - fj / fDiscLong;

            vertex.normal.x = std::sin(fi * phi) * cosTheta;
            vertex.normal.y = sinTheta;
            vertex.normal.z = std::cos(fi * phi) * cosTheta;

            vertex.position = radius * vertex.normal;

            data.push_back(vertex);
        }
    }

    std::vector<ShapeVertex> vertices{};
    // Construit les vertex finaux en regroupant les données en triangles :
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme :
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for (size_t j = 0; j < discLong; ++j)
    {
        const size_t offset = j * (discLat + 1);
        for (size_t i = 0; i < discLat; ++i)
        {
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + (i + 1)]);
            vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            vertices.push_back(data[offset + i + discLat + 1]);
        }
    }

    return vertices;
}

} // namespace glimac
