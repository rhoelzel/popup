#include <iostream>
#include "gui/application.h"

extern "C"
{

  void up(double interval);
  void down();
  
  void up(double interval)
  {
    Application::get_instance()->loop(interval);
  }
  
  void down()
  {
    Application::get_instance()->terminate();
  }
}
