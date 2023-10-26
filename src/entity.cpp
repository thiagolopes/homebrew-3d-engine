#include "entity.hh"
#include "glm/gtc/matrix_transform.hpp"
#include <cassert>

#define DEGRESS 360.0f

Entity::Entity(Mesh *mesh, Material *material, float position, float scale, float angle_degrees)
  : _mesh(mesh), _material(material), _position(position), _word_position(0.0f), _scale(scale),
      _angle_degrees(angle_degrees), _rotation_dir(0.0f, 1.0f, 0.0f){};

Entity::Entity(Model &model, float position, float scale, float angle_degrees)
  : _mesh(model.get_mesh()), _material(model.get_material()), _position(position), _word_position(0.0f), _scale(scale),
      _angle_degrees(angle_degrees), _rotation_dir(0.0f, 1.0f, 0.0f){};

void
Entity::inc_position(float x, float y, float z)
{
  _position.x += x;
  _position.y += y;
  _position.z += z;
}

void
Entity::position(float x, float y, float z)
{
  _position.x = x;
  _position.y = y;
  _position.z = z;
}

void
Entity::inc_scale(float scale)
{
  _scale.x += scale;
  _scale.y += scale;
  _scale.z += scale;
}

void
Entity::scale(float scale)
{
  _scale.x = scale;
  _scale.y = scale;
  _scale.z = scale;
}

void
Entity::scale(float scale_x, float scale_y, float scale_z)
{
  _scale.x = scale_x;
  _scale.y = scale_y;
  _scale.z = scale_z;
}

void
Entity::inc_angle(float angle)
{
  assert(angle < DEGRESS);
  _angle_degrees += angle;

  if (_angle_degrees > DEGRESS)
    _angle_degrees -= DEGRESS;
  if (_angle_degrees < 0)
    _angle_degrees += DEGRESS;
}

void
Entity::set_rotation_dir(float x, float y, float z){
  _rotation_dir.x = x;
  _rotation_dir.y = y;
  _rotation_dir.z = z;
}

void
Entity::angle(float angle)
{
  assert(angle < DEGRESS);
  _angle_degrees = angle;
}

glm::mat4 &
Entity::get_model_position()
{
  _word_position = glm::translate(glm::mat4(1.0f), _position);
  _word_position = glm::scale(_word_position, _scale);
  _word_position = glm::rotate(_word_position, glm::radians(_angle_degrees), _rotation_dir);

  return _word_position;
}
