#include "cone_vertices.hpp"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <vector>
#include "common.hpp"

namespace glimac {

std::vector<ShapeVertex> cone_vertices(float height, float radius, size_t discLat, size_t discHeight) // NOLINT(bugprone-easily-swappable-parameters, readability-inconsistent-declaration-parameter-name)
{
    // Equation paramétrique en (r, phi, h) du cône
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= h <= height
    //
    // x(r, phi, h) = r (height - h) sin(phi) / height
    // y(r, phi, h) = h
    // z(r, phi, h) = r (height - h) cos(phi) / height
    //
    // Discrétisation:
    // phi = 2PI / discLat, h = height / discHeight
    //
    // x(r, i, j) = r * (height - j * h) * sin(i * phi) / height
    // y(r, i, j) = j * h
    // z(r, i, j) = r * (height - j * h) * cos(i * phi) / height
    //
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs

    const auto fDiscLat    = static_cast<float>(discLat);
    const auto fDiscHeight = static_cast<float>(discHeight);

    const float phi = 2.f * glm::pi<float>() / fDiscLat;
    const float h   = height / fDiscHeight;

    std::vector<ShapeVertex> data;

    // Construit l'ensemble des vertex
    for (size_t j = 0; j <= discHeight; ++j)
    {
        for (size_t i = 0; i < discLat; ++i)
        {
            const auto fj = static_cast<float>(j);
            const auto fi = static_cast<float>(i);

            ShapeVertex vertex{};

            vertex.texCoords.x = fi / fDiscLat;
            vertex.texCoords.y = fj / fDiscHeight;

            vertex.position.x = radius * (height - fj * h) * std::sin(fi * phi) / height;
            vertex.position.y = fj * h;
            vertex.position.z = radius * (height - fj * h) * std::cos(fi * phi) / height;

            vertex.normal.x = std::sin(fi * phi);
            vertex.normal.y = radius / height;
            vertex.normal.z = std::cos(fi * phi);
            vertex.normal   = glm::normalize(vertex.normal);

            data.push_back(vertex);
        }
    }

    std::vector<ShapeVertex> vertices{};
    // Construit les vertex finaux en regroupant les données en triangles :
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme :
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for (size_t j = 0; j < discHeight; ++j)
    {
        const size_t offset = j * discLat;
        for (size_t i = 0; i < discLat; ++i)
        {
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + (i + 1) % discLat]);
            vertices.push_back(data[offset + discLat + (i + 1) % discLat]);
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + discLat + (i + 1) % discLat]);
            vertices.push_back(data[offset + i + discLat]);
        }
    }

    return vertices;
}

} // namespace glimac
