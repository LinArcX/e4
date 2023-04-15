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
    void LoadFonts(ImGuiIO& io);

    void HandleEvents();

    void Draw();
    void ShowMainPanel();

    void RenderScene();

    void ShowMenuBar();
    void ShowRandomText();
    void AboutApp(bool* p_open);
    void QuitApp();
    void OpenFileDialog();

  public:
    void Loop();
    void CleanUp();

  private:
    const char* glsl_version;

    SDL_Window* window;
    SDL_GLContext gl_context;

    int viewPortX;
    int viewPortY;

    bool appIsRunning = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool panelHasCloseButton = true;

    static bool about_app;
};

#endif
