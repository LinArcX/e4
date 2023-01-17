#include "ui/application.hpp"

int main(int, char**)
{
  Application app;

  if(int retValue = app.Initialize() < 0)
    return retValue;
  app.Loop();
  app.CleanUp();

  return 0;
}
