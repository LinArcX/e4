#include "application.hpp"

// Documentations:
// 1. from the docs/ folder
// 2. from top top of imgui.cpp.
// 3. from online: https://github.com/ocornut/imgui/tree/master/docs

#include "SDL_keycode.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"
#include <stdio.h>

#include "fonts/InconsolateMedium.h"
#include "fonts/UbuntuMedium.h"
#include "fonts/FiraSansMedium.h"

#include "fonts/MerriWeatherRegular.h"
#include "fonts/SofiaSansRegular.h"
#include "fonts/SofiaSansSemiCondensedRegular.h"
#include "fonts/AlegreyaMedium.h"
#include "imgui_internal.h"

bool Application::about_app = false;

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
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  LoadFonts(io);
}

void Application::LoadFonts(ImGuiIO& io)
{
  ImFontConfig fontConfig;

  static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 };
  ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(InconsolateMedium_compressed_data, InconsolateMedium_compressed_size, 16.0f, NULL, ranges);

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
      appIsRunning = false;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
      appIsRunning = false;
    if (event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.mod & KMOD_CTRL)
      {
        if(event.key.keysym.sym == SDLK_q)
        {
          QuitApp();
        }
        if(event.key.keysym.sym == SDLK_a)
        {
          //AboutApp(&about_app);
        }
      }
    }
  }
}

void Application::AboutApp(bool* p_open)
{
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
  if (!ImGui::Begin("About Kindd", p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
  {
      ImGui::End();
      return;
  }

  ImGui::Text("Kindd: 2.2.0");
  ImGui::Separator();
  ImGui::Text("By LinArcX and other contributors.");
  ImGui::Text("Kindd is licensed under the  GPL-3.0 License, see LICENSE for more information.");

  ImGui::End();
}

void Application::QuitApp()
{
  appIsRunning = false;
}

void Application::OpenFileDialog()
{
  if (ImGui::Button("Open File"))
  {
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(650, 450), ImGuiCond_FirstUseEver);
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
  }

  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
  {
    if (ImGuiFileDialog::Instance()->IsOk())
    {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
    }
    ImGuiFileDialog::Instance()->Close();
  }
}

void Application::ShowMenuBar()
{
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Kindd"))
    {
      ImGui::MenuItem("About", NULL, &about_app);
      if (ImGui::MenuItem("Quit", "CTRL+E")) { QuitApp(); }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

void Application::ShowRandomText()
{
  ImGui::Text("INPUT FILE(.ISO, etc..)");
  OpenFileDialog();
  static int selected_node = 0;  // Index of the currently selected node

  // Using those as a base value to create width/height that are factor of the size of our font
  const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
  const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

  if (ImGui::TreeNode("Tree view"))
  {
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

    ImGui::SetItemDefaultFocus();
    if (ImGui::BeginTable("3ways", 3, flags))
    {
        // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        ImGui::TableHeadersRow();

        // Simple storage to output a dummy file-system.
        struct MyTreeNode
        {
            const char*     Name;
            const char*     Type;
            int             Size;
            int             ChildIdx;
            int             ChildCount;
            static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes, int &selected_node)
            {
              //if (ImGui::IsItemActive() || ImGui::IsItemHovered())
              //{
              //  ImGui::SetTooltip("Bang!");
              //}

              if (ImGui::IsKeyPressed(ImGuiKey_J))
              {
                  ImGui::SetTooltip("J Pressed!");
                  selected_node++;
              }
              if (ImGui::IsKeyPressed(ImGuiKey_K))
              {
                  ImGui::SetTooltip("K Pressed!");
                  selected_node--;
              }
              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              const bool is_folder = (node->ChildCount > 0);
              if (is_folder)
              {
                  if (selected_node == node->ChildIdx)
                  {
                    ImGui::SetScrollHereY(1.0f);  // Scroll to the selected node
                    //ImGui::SetTooltip("J Pressed!");
                  }

                  bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
                  ImGui::TableNextColumn();
                  ImGui::TextDisabled("--");
                  ImGui::TableNextColumn();
                  ImGui::TextUnformatted(node->Type);
                  if (open)
                  {
                      for (int child_n = 0; child_n < node->ChildCount; child_n++)
                          DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes, selected_node);
                      ImGui::TreePop();
                  }
              }
              else
              {
                  //ImGui::SetScrollHereY(1.0f);
                  if (selected_node == node->ChildIdx)
                  {
                    ImGui::SetTooltip("J Pressed!");
                      //ImGui::SetScrollHere();  // Scroll to the selected node
                  }
                  ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                  ImGui::TableNextColumn();
                  ImGui::Text("%d", node->Size);
                  ImGui::TableNextColumn();
                  ImGui::TextUnformatted(node->Type);
              }
            }
        };
        static const MyTreeNode nodes[] =
        {
            { "Root",                         "Folder",       -1,       1, 3    }, // 0
            { "Music",                        "Folder",       -1,       4, 2    }, // 1
            { "Textures",                     "Folder",       -1,       6, 3    }, // 2
            { "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
            { "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
            { "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
            { "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
            { "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
            { "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
        };

        MyTreeNode::DisplayNode(&nodes[0], nodes, selected_node);

        ImGui::EndTable();

        //if (ImGui::IsItemActive() || ImGui::IsItemHovered())
        //{
          //if (ImGui::IsKeyPressed(ImGuiKey_J))
          //{
          //  ImGui::SetTooltip("J Pressed!");
          //  selected_node++;
          //}
          //if (ImGui::IsKeyPressed(ImGuiKey_K))
          //{
          //  ImGui::SetTooltip("K Pressed!");
          //  selected_node--;
          //}
        //}
        }

    ImGui::TreePop();

    //if (ImGui::IsKeyPressed(ImGuiKey_Space))
    //{
    //  ImGui::SetTooltip("Bang!");
    //}
  }

  //ImGui::Separator();

  //ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
  //ImGui::BulletText("The \"Examples\" menu above leads to more demo contents.");
  //ImGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
  //                  "and Metrics/Debugger (general purpose Dear ImGui debugging tool).");

  //ImGui::Text("PROGRAMMER GUIDE:");
  //ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
  //ImGui::BulletText("See comments in imgui.cpp.");
  //ImGui::BulletText("See example applications in the examples/ folder.");
  //ImGui::BulletText("Read the FAQ at http://www.dearimgui.org/faq/");
  //ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
  //ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
  //ImGui::Separator();

  //ImGui::Text("USER GUIDE:");
  //ImGui::ShowUserGuide();
}

void Application::Draw()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if (about_app)
    Application::AboutApp(&about_app);

  //ImGui::ShowDemoWindow();
  ShowMainPanel();

  ImGui::EndFrame();
}

void Application::ShowMainPanel()
{
  #ifdef IMGUI_HAS_VIEWPORT
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->GetWorkPos());
  ImGui::SetNextWindowSize(viewport->GetWorkSize());
  ImGui::SetNextWindowViewport(viewport->ID);
  #else
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  #endif

  if(!ImGui::Begin("Dear ImGui Demo", &panelHasCloseButton,
                   ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
  {
    ImGui::End();
    return;
  }

  ShowMenuBar();
  ShowRandomText();

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
  while (appIsRunning)
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
