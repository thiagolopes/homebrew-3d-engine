#include "camera.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <math.h>

#include <iostream>
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov)
    : m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(glm::vec3(0.0f, 0.0f, 0.0f)),
      m_right(glm::vec3(0.0f, 0.0f, 0.0f)), m_word_up(up), m_yaw(yaw), m_pitch(pitch), m_fov(fov) {
  update_camera();
};

glm::mat4 Camera::get_camera_matrix() { return glm::lookAt(m_position, m_position + m_front, m_up); };

void Camera::process_keyboard(camera_direction_t d, float delta_time) {
  float velocity = m_moviment_speed * delta_time;

  switch (d) {
  case camera_direction_t::FORWARD:
    m_position += m_front * velocity;
    break;
  case camera_direction_t::BACKWARD:
    m_position -= m_front * velocity;
    break;
  case camera_direction_t::RIGHT:
    m_position += m_right * velocity;
    break;
  case camera_direction_t::LEFT:
    m_position -= m_right * velocity;
    break;
  }
};

void Camera::update_camera() {
  glm::vec3 new_front;
  new_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  new_front.y = sin(glm::radians(m_pitch));
  new_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(new_front);

  m_right = glm::normalize(glm::cross(m_front, m_word_up));
  m_up = glm::normalize(glm::cross(m_right, m_front));
};
