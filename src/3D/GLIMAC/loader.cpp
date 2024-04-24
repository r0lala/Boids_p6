#include "loader.h"

Object3D loadOBJ(const char * path)
{
    Object3D toreturn;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<float> positions;

    std::cout << tinyobj::LoadObj(shapes, materials, path);

    for(auto s : shapes[0].mesh.positions) positions.push_back(s);
    for(auto i : shapes[0].mesh.indices)
    {
        vertex tmp;
        tmp.position = glm::vec3(positions[i*3], positions[i*3 + 1], positions[i*3+2]);
        tmp.normal = glm::vec3(shapes[0].mesh.normals[i*3], shapes[0].mesh.normals[i*3+1], shapes[0].mesh.normals[i*3+2]);
        tmp.uv = glm::vec2(shapes[0].mesh.texcoords[i*2], shapes[0].mesh.texcoords[i*2+1]);
        toreturn.vertices.push_back(tmp);
    } 
    return toreturn;
}