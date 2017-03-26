#ifndef __SCIGMA_GUI_GLUTIL_H__
#define __SCIGMA_GUI_GLUTIL_H__

#include <sstream>
#include <iostream>
#include "application.h"

#define GLERR push_opengl_error(__FILE__,__LINE__)


    inline void push_opengl_error(const char* file, int line)
    {
      unsigned int code;
      while ((code=glGetError())!= GL_NO_ERROR)
	{
	  std::ostringstream combine;
	  combine<<"OpenGL error code "<<std::hex<<code<<std::dec;
	  Application::get_instance()->push_error(combine.str(),file,line);
	  std::cout<<combine.str()<<file<<line<<std::endl;
	}
    }

    inline void push_shader_compile_error(GLuint shader, const char* file, int line)
    {
      char log[0x1000]; GLsizei length;
      std::ostringstream combine;
      glGetShaderInfoLog(shader,0x1000,&length,log);
      if(length)
	{
	  log[length]=0;
	  combine<<"shader compilation output: "<<log; 
	  Application::get_instance()->push_error(combine.str(),file,line);
	  //	  throw(combine.str());
	}
    }

    inline void push_program_link_error(GLuint program, const char* file, int line)
    {
      char log[0x1000]; GLsizei length;
      std::ostringstream combine;
      glGetProgramInfoLog(program,0x1000,&length,log);
      if(length)
	{
	  log[length]=0;
	  combine<<"program linking output: "<<log; 
	  Application::get_instance()->push_error(combine.str(),file,line);
	  //throw(combine.str());
	}
    }

#endif /* __SCIGMA_GUI_GLUTIL_H__ */
