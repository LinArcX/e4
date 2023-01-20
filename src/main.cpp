#include "ui/application.hpp"

int main(int, char**)
{
  Application app;
  app.Initialize();

  if(int initResult = app.Initialize() < 0)
    return initResult;
  app.Loop();
  app.CleanUp();

  return 0;
}
