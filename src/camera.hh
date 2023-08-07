#pragma once
// camera movimentent abstration
#include <glm/glm.hpp>

enum camera_direction_t { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
 private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_word_up;

  float m_yaw;
  float m_pitch;
  float m_fov;
  float m_moviment_speed = 2.5f;
  float m_mouse_sensitivity = 0.1f;

  void update_camera();

 public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = -90.0f, float pitch = 0.0f, float fov = 45.0f);

  void process_mouse_moviment(float x_offset, float y_offset, bool constrian_pitch = true);
  void process_mouse_scroll(float y_offset);
  void process_keyboard(camera_direction_t d, float delta_time);

  glm::mat4 get_camera_matrix();

  inline float get_fov() { return m_fov; };
  inline glm::vec3 &get_position() { return m_position; };
  inline void set_moviment_speed(float m) { m_moviment_speed = m; };
};
