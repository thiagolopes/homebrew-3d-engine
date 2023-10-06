#pragma once
// camera movimentent abstration
#include <glm/glm.hpp>
#include "inputs.hh"

class Camera
{
private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;
  glm::vec3 _right;
  glm::vec3 _direction_up;

  glm::mat4 _perspecitve_view;
  glm::mat4 _perspecitve_projection;

  float _yaw;
  float _pitch;
  float _fov;
  float _moviment_speed = 2.5f;
  float _mouse_sensitivity = 0.1f;
  float _zoom_sensitivity = 2.0f;

  float _view_width;
  float _view_height;

  float _z_near;
  float _z_far;

  void update_camera();
  void process_keyboard(Keyboard &k, float delta_time);

public:
  Camera(float position, float width, float height, float up = 0.0f, float yaw = -90.0f, float pitch = 0.0f,
         float fov = 45.0f, float z_zear = 0.1f, float z_far = 100.0f);

  void process_mouse_moviment(Mouse &m, bool constrian_pitch = true);
  void process_mouse_scroll(float y_offset);

  void update(Keyboard &k, Mouse &m, float delta_time);

  glm::mat4 &get_view_matrix();
  glm::mat4 &get_projection();

  inline float
  get_fov()
  {
    return _fov;
  };
  inline glm::vec3 &
  get_position()
  {
    return _position;
  };
  inline void
  set_moviment_speed(float m)
  {
    _moviment_speed = m;
  };
};
