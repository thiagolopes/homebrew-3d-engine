#include "window.hh"

#include "inputs.hh"
#include "mesh.hh"
#include "opengl_middleware.hh"

Window::Window(char *window_name, float width, float height)
    : m_window(nullptr), m_width(width), m_height(height), m_deltatime(0), m_lastframe(0)
{
  /* Initialize the library */
  if (!glfwInit())
    {
      std::cerr << "[ERROR] In load GLFW!" << std::endl;
      std::exit(EXIT_FAILURE);
    }

  // set opengl to 3 and use CORE. this will make VAO especification mandatory
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  m_window = glfwCreateWindow(m_width, m_height, window_name, nullptr, nullptr);
  if (!m_window)
    {
      std::cerr << "[ERROR] Fail in create window GLFW!" << std::endl;
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_window);
  // init glew
  if (glewInit() != GLEW_OK)
    {
      std::cout << "[ERROR] Fail in load graphical drive" << std::endl;
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }

  // show opengl version
  std::cout << "[LOG]" << glGetString(GL_VERSION) << std::endl;
  update_deltatime_frame();

  // setup minors options by default
  glfwSetKeyCallback(get_window(), (GLFWkeyfun)Keyboard::get_instance().glfw_callback);
  OpenGLCallback &op = OpenGLCallback::get_instance();
  op.set_window(get_window());
};

Window::~Window() { glfwTerminate(); };

void
Window::set_mouse_scroll_callback(void *f)
{
  glfwSetScrollCallback(get_window(), (GLFWscrollfun)f);
};

void
Window::set_mouse_button_callback(void *f)
{
  glfwSetMouseButtonCallback(get_window(), (GLFWmousebuttonfun)f);
};

void
Window::set_mouse_moviment_callback(void *f)
{
  glfwSetCursorPosCallback(get_window(), (GLFWcursorposfun)f);
};

float
Window::get_time()
{
  return glfwGetTime();
}

void
Window::end_frame()
{
  update_deltatime_frame();
  glfwSwapBuffers(m_window);
  glfwPollEvents();
};

// set a running
bool
Window::running()
{
  return !glfwWindowShouldClose(m_window);
};

void
Window::begin_frame() const
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};
