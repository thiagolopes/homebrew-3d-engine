#include "camera.hh"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <math.h>

#include <iostream>
#include <ostream>
Camera::Camera(glm::vec3 position, float width, float height, glm::vec3 up, float yaw, float pitch, float fov,
               float z_near, float z_far)
    : _position(position),
      _front(glm::vec3(0.0f, 0.0f, -1.0f)),
      _up(glm::vec3(0.0f, 0.0f, 0.0f)),
      _right(glm::vec3(0.0f, 0.0f, 0.0f)),
      _word_up(up),
      _yaw(yaw),
      _pitch(pitch),
      _fov(fov),
      _view_width(width),
      _view_height(height),
      _z_near(z_near),
      _z_far(z_far) {
  update_camera();
};

glm::mat4 Camera::get_view_matrix() { return glm::lookAt(_position, _position + _front, _up); };

void Camera::process_mouse_moviment(float x_offset, float y_offset, bool constrian_pitch) {
  x_offset *= _mouse_sensitivity;
  y_offset *= _mouse_sensitivity;

  _yaw += x_offset;
  _pitch += y_offset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrian_pitch) {
    if (_pitch > 89.0f)
      _pitch = 89.0f;
    if (_pitch < -89.0f)
      _pitch = -89.0f;
  }
  update_camera();
}

void Camera::process_mouse_scroll(float y_offset) {
  _fov -= (float)y_offset;
  if (_fov < 1.0f)
    _fov = 1.0f;
  if (_fov > 45.0f)
    _fov = 45.0f;
}

void Camera::process_keyboard(camera_direction_t d, float delta_time) {
  float velocity = _moviment_speed * delta_time;

  switch (d) {
    case camera_direction_t::FORWARD:
      _position += _front * velocity;
      break;
    case camera_direction_t::BACKWARD:
      _position -= _front * velocity;
      break;
    case camera_direction_t::RIGHT:
      _position += _right * velocity;
      break;
    case camera_direction_t::LEFT:
      _position -= _right * velocity;
      break;
  }
};

void Camera::update_camera() {
  glm::vec3 new_front;
  new_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
  new_front.y = sin(glm::radians(_pitch));
  new_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
  _front = glm::normalize(new_front);

  _right = glm::normalize(glm::cross(_front, _word_up));
  _up = glm::normalize(glm::cross(_right, _front));
};

glm::mat4 Camera::get_projection() {
  return glm::perspective(glm::radians(get_fov()), _view_width / _view_height, _z_near, _z_far);
}
