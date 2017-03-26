#include <iostream>
#include "application.h"
#include "glutil.h"

extern "C"
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"  
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wold-style-cast"
  void(*python_callback_)(int,int)=NULL;

  void set_python_callback(void(*python_callback)(int,int))
  {
    python_callback_=python_callback;
    std::cout<<(void *)(python_callback)<<std::endl;
  }
  
  void window_close_callback(GLFWwindow* w)
  {
    std::cout<<"Window closed!"<<std::endl;
    Application::get_instance()->terminate();
  }
  
  void window_size_callback(GLFWwindow* w, int width, int height)
  {
    std::cout<<"Window resized!"<<std::endl;
  }

  void mouse_button_callback(GLFWwindow* w, int button , int action, int mods)
  {
    /*    std::cout<<"Mouse button pressed!"<<std::endl;
	  std::cout<<(void *)(python_callback_)<<std::endl;*/
    python_callback_(10,20);
  }
  
  void cursor_position_callback(GLFWwindow* w, double x, double y)
  {
    std::cout<<"Cursor moved!!"<<std::endl;
  }
  
  void scroll_callback(GLFWwindow* w, double xScroll, double yScroll)
  {
    std::cout<<"Scrolling!"<<std::endl;
  }
  
#pragma clang diagnostic pop  
} /* end extern "C" block */

Application* Application::theInstance_(NULL);
    
Application::Application():masterWindow_(NULL),idleIndex_(0),loopIsRunning_(false)
    {}
    
    Application::~Application()
    {}

    Application* Application::get_instance()
    {
      if(theInstance_)
	return theInstance_;
      
      theInstance_->initialize();
      return theInstance_;
    }

void Application::push_error(std::string error, const char* file, int line)
{
  std::cout<<"Error '"<<error<<"' in file '"<<file<<"' on line "<<line<<std::endl;
}

    void Application::push_glfw_error(int code, const char* error)
    {
      std::cout<<"GLFW error code "<<code<<": "<<error<<std::endl; 
    }


    void Application::initialize()
    {
      if(theInstance_)
	return;
      theInstance_ = new Application();
      
      glfwSetErrorCallback(push_glfw_error);
      if (!glfwInit())
	return;
#ifdef SCIGMA_USE_OPENGL_3_2
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
      glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#else
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
#endif
      //      glfwWindowHint(GLFW_VISIBLE,GL_FALSE);
      //glfwWindowHint(GLFW_DECORATED,GL_FALSE);
      theInstance_->masterWindow_=glfwCreateWindow(100,100, "", NULL,NULL);
      glfwWindowHint(GLFW_VISIBLE,GL_TRUE);
      //glfwWindowHint(GLFW_SAMPLES, 4);
      glfwMakeContextCurrent(theInstance_->masterWindow_);
      GLERR;

      glewExperimental = GL_TRUE; 
      GLenum err = glewInit();
      if(GLEW_OK!=err)
	std::cout<<"could not initialize GLEW library"<<std::endl;
      glGetError(); // glewInit() apparently sometimes produces a benign invalid enum error 
      // catch that here
      glFinish();
      GLERR;
      glfw_window_set_callbacks(theInstance_->masterWindow_);
      atexit(terminate_on_end);
    }
    
    void Application::terminate()
    {
      if(!theInstance_)
	return;

      glfwDestroyWindow(theInstance_->masterWindow_);
      glfwTerminate();
      
      delete theInstance_;
      theInstance_=NULL;
    }

void Application::terminate_on_end()
{
  if(!theInstance_)
    return;
  get_instance()->terminate();
}

    void Application::glfw_window_set_callbacks(GLFWwindow* w)
    {
      glfwSetWindowCloseCallback(w, window_close_callback);  
      glfwSetWindowSizeCallback(w, window_size_callback);
      glfwSetMouseButtonCallback(w, mouse_button_callback);
      glfwSetCursorPosCallback(w, cursor_position_callback);
      glfwSetScrollCallback(w, scroll_callback);
    }
    
    GLFWwindow* Application::get_master_window()
    {
      return masterWindow_;
    }
    
    void Application::loop(double seconds)
    {
      if(seconds<0)
	return;
      glfwWaitEventsWithTimeOut(seconds);
    }
    
    void Application::break_loop()
    {
      loopIsRunning_=false;
    }
