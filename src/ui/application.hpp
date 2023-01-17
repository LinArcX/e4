#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "imgui.h"
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

class Application
{
  public:
    Application();
    ~Application();
    int Initialize();

  private:
    void CheckSdlVersion();
    void CreateWindow();

    void SetupImgui();
    void LoadFonts();

    void HandleEvents();
    void Draw();
    void RenderScene();

  public:
    void Loop();
    void CleanUp();

  private:
    const char* glsl_version;

    SDL_Window* window;
    SDL_GLContext gl_context;

    int viewPortX;
    int viewPortY;

    bool appIsRunning = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

#endif
