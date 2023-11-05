#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CubeModel {
public:
    static glm::vec3 PX_POS[6];
    static glm::vec3 NX_POS[6];
    static glm::vec3 PY_POS[6];
    static glm::vec3 NY_POS[6];
    static glm::vec3 PZ_POS[6];
    static glm::vec3 NZ_POS[6];
    static  glm::vec3 NORMALS[6];
    static  glm::vec2 UV[6];
    static float uv[48];
    //CubeModel();
};
