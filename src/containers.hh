#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "buffers.hh"

namespace Container {
std::vector<Vertex> positions = {
    {
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
    },  // A 0 NORMAL A
    {
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
    },  // B 1 NORMAL A
    {
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
    },  // C 2 NORMAL A
    {
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
    },  // D 3 NORMAL A
    {
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    },  // E 4 NORMAL B
    {
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    },  // F 5 NORMAL B
    {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    },  // G 6 NORMAL B
    {
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    },  // H 7 NORMAL B
    {
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
    },  // D 8  NORMAL C
    {
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
    },  // A 9  NORMAL C
    {
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
    },  // E 10 NORMAL C
    {
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
    },  // H 11 NORMAL C
    {
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    },  // B 12 NORMAL D
    {
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    },  // C 13 NORMAL D
    {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    },  // G 14 NORMAL D
    {
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    },  // F 15 NORMAL D
    {
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    },  // A 16 NORMAL E
    {
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    },  // B 17 NORMAL E
    {
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    },  // F 18 NORMAL E
    {
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    },  // E 19 NORMAL E
    {
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    },  // C 20 NORMAL F
    {
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec2(1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    },  // D 21 NORMAL F
    {
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec2(1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    },  // H 22 NORMAL F
    {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec2(0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    },  // G 23 NORMAL F
};

std::vector<unsigned int> indices = {
    // front
    0, 3, 2, 2, 1, 0,
    // back
    4, 5, 6, 6, 7, 4,
    // left
    11, 8, 9, 9, 10, 11,
    //  right
    12, 13, 14, 14, 15, 12,
    // bottom
    16, 17, 18, 18, 19, 16,
    // top
    20, 21, 22, 22, 23, 20};
}  // namespace Container
