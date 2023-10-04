#pragma once
// camera movimentent abstration
#include <glm/glm.hpp>

enum camera_direction_t { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
 private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;
  glm::vec3 _right;
  glm::vec3 _word_up;

  float _yaw;
  float _pitch;
  float _fov;
  float _moviment_speed = 2.5f;
  float _mouse_sensitivity = 0.1f;

  float _view_width;
  float _view_height;

  float _z_near;
  float _z_far;

  void update_camera();

 public:
  Camera(glm::vec3 position, float width, float height, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
         float pitch = 0.0f, float fov = 45.0f, float z_zear = 0.1f, float z_far = 100.0f);

  void process_mouse_moviment(float x_offset, float y_offset, bool constrian_pitch = true);
  void process_mouse_scroll(float y_offset);
  void process_keyboard(camera_direction_t d, float delta_time);

  glm::mat4 get_view_matrix();
  glm::mat4 get_projection();

  inline float get_fov() { return _fov; };
  inline glm::vec3 &get_position() { return _position; };
  inline void set_moviment_speed(float m) { _moviment_speed = m; };
};
