#include "entity.hh"
#include "glm/gtc/matrix_transform.hpp"
#include <cassert>
#define DEGRESS 360.0f

Entity::Entity(Mesh *mesh, Material *material, float position, float scale, float angle_degrees)
    : _mesh(mesh),
      _material(material),
      _position(position),
      _word_position(0.0f),
      _scale(scale),
      _angle_degrees(angle_degrees){};

Entity::Entity(Model &model, float position, float scale, float angle_degrees)
    : _mesh(model.mesh),
      _material(model.material),
      _position(position),
      _word_position(0.0f),
      _scale(scale),
      _angle_degrees(angle_degrees){};

void Entity::inc_position(float x, float y, float z) {
  _position.x += x;
  _position.y += y;
  _position.z += z;
}

void Entity::position(float x, float y, float z) {
  _position.x = x;
  _position.y = y;
  _position.z = z;
}

void Entity::inc_scale(float scale) {
  _scale.x += scale;
  _scale.y += scale;
  _scale.z += scale;
}

void Entity::scale(float scale) {
  _scale.x = scale;
  _scale.y = scale;
  _scale.z = scale;
}

void Entity::scale(float scale_x, float scale_y, float scale_z) {
  _scale.x = scale_x;
  _scale.y = scale_y;
  _scale.z = scale_z;
}

void Entity::inc_angle(float angle) {
  assert(angle < DEGRESS);
  _angle_degrees += angle;

  if (_angle_degrees > DEGRESS)
    _angle_degrees -= DEGRESS;
  if (_angle_degrees < 0)
    _angle_degrees += DEGRESS;
}

void Entity::angle(float angle) {
  assert(angle < DEGRESS);
  _angle_degrees = angle;
}

glm::mat4 &Entity::word_position() {
  _word_position = glm::translate(glm::mat4(1.0f), _position);
  _word_position = glm::scale(_word_position, _scale);
  _word_position = glm::rotate(_word_position, glm::radians(_angle_degrees), glm::vec3(0.0, 1.0, 0.0));

  return _word_position;
}
