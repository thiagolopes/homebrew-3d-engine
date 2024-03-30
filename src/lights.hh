#pragma once

#include <glm/glm.hpp>
#include "imgui.h"
#include "vendor/imgui/imgui.h"

// types os lights:
struct PointLight {
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight(float position, float ambient, float diffuse, float specular, float constant = 0.0f, float linear = 0.0f,
               float quadratic = 0.0f)
        : position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant),
          linear(linear), quadratic(quadratic){};

    void set_position(float x, float y, float z) {
        position.x = x;
        position.y = y;
        position.z = z;
    }
};

struct DirLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirLight(float direction, float ambient, float diffuse, float specular)
        : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular){};

    void set_direction(float x, float y, float z) {
        direction.x = x;
        direction.y = y;
        direction.z = z;
    }
};
