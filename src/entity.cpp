#include "entity.hh"
#include "glm/gtc/matrix_transform.hpp"
#include <cassert>
#define DEGRESS 360.0f

Entity::Entity(Mesh *mesh, Material *material, glm::vec3 position, float scale, float angle)
    : _mesh(mesh), _material(material), _position(position), _word_position(0.0), _scale(scale), _angle(angle){};

void Entity::inc_position(float x, float y, float z) {
  _position.x += x;
  _position.y += y;
  _position.z += z;
}

void Entity::inc_scale(float scale) {
  _scale.x += scale;
  _scale.y += scale;
  _scale.z += scale;
}

void Entity::inc_angle(float angle) {
  assert(angle < DEGRESS);
  _angle += angle;

  if (_angle > DEGRESS)
    _angle -= DEGRESS;
  if (_angle < 0)
    _angle += DEGRESS;
}

glm::mat4 &Entity::word_position() {
  _word_position = glm::translate(glm::mat4(1.0f), _position);
  _word_position = glm::scale(_word_position, _scale);
  _word_position = glm::rotate(_word_position, _angle, glm::vec3(0.0, 1.0, 0.0));

  return _word_position;
}
