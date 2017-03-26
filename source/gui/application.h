#ifndef __SCIGMA_GUI_APPLICATION_H__
#define __SCIGMA_GUI_APPLICATION_H__

#define GLEW_STATIC

#include <glew.h>
#include <glfw3.h>

class Application
{
 public:
  static Application* get_instance();
  void loop(double seconds);
  void break_loop();
  void push_error(std::string error, const char* file=NULL, int line=0);

  void initialize();
  void terminate();
  
 private:
  Application();
  ~Application();
  Application(const Application&);
  Application& operator=(const Application&);

  static void terminate_on_end();
    
  GLFWwindow* get_master_window();
  void glfw_window_set_callbacks(GLFWwindow* ptr);
      
  static void push_glfw_error(int code, const char* error);
  static Application* theInstance_;
  
  GLFWwindow* masterWindow_;
  
  static constexpr double REFRESH_INTERVAL=1/60.;
  size_t idleIndex_;
  bool loopIsRunning_;
  char padded[7];
};
    
#endif /* __SCIGMA_GUI_APPLICATION_H__ */
