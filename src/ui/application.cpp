#include "application.hpp"

// Documentations:
// 1. from the docs/ folder
// 2. from top top of imgui.cpp.
// 3. from online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "fonts/RobotoRegular.h"
#include "fonts/UbuntuMedium.h"

Application::Application()
{}

Application::~Application()
{}

int Application::Initialize()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }
  CheckSdlVersion();
  CreateWindow();
  SetupImgui();

  return 0;
}

void Application::CheckSdlVersion()
{
  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  glsl_version = "#version 150";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
}

void Application::CreateWindow()
{
  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
}

void Application::SetupImgui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  viewPortX = (int)io.DisplaySize.x;
  viewPortY = (int)io.DisplaySize.y;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  LoadFonts(io);
}

void Application::LoadFonts(ImGuiIO& io)
{
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !

  ImFontConfig fontConfig;
	//fontConfig.FontDataOwnedByAtlas = false;
	ImFont* font = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
	//ImFont* font = io.Fonts->AddFontFromMemoryTTF((void*)UbuntuMedium_compressed_data, UbuntuMedium_compressed_size, 16.0f, &fontConfig);
	io.FontDefault = font;
}

void Application::HandleEvents()
{
  // Poll and handle events (inputs, window resize, etc.)
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
  // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
      appIsRunning = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
      appIsRunning = true;
  }
}

void Application::Draw()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();
  //ShowMainPanel();

  ImGui::EndFrame();
}

void Application::ShowMainPanel()
{
  ImGuiWindowFlags window_flags = 0;
  //window_flags |= ImGuiWindowFlags_MenuBar;
  window_flags |= ImGuiWindowFlags_NoNav;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

  if (!ImGui::Begin("Dear ImGui Demo", &panelHasCloseButton, window_flags))
  {
      ImGui::End();
      return;
  }
  ImGui::End();
}

void Application::RenderScene()
{
  ImGui::Render();
  glViewport(0, 0, viewPortX, viewPortY);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
}

void Application::Loop()
{
  while (!appIsRunning)
  {
    HandleEvents();
    Draw();
    RenderScene();
  }
}

void Application::CleanUp()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
