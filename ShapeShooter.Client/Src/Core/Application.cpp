#include "Application.h"
#include <Core/Core.h>

#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

static void InitImGuiStyles()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
    colors[ImGuiCol_Separator] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    colors[ImGuiCol_Tab] = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
    colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.47f, 0.47f, 0.47f, 0.47f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style->ScaleAllSizes(2.0f);
}

static void InitializeWindow(SDL_Window*& window, SDL_GLContext& gl_context, int windowW, int windowH)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL2 video subsystem couldn't be initialized. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow(
        "Shape_Shooter++",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowW, windowH,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);

    SDL_SetWindowResizable(window, (SDL_bool)true);


    gl_context = SDL_GL_CreateContext(window);


    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }

    std::cout << "OpenGL version loaded: " << GLVersion.major << "." << GLVersion.minor << std::endl;


    glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    glEnable(GL_MULTISAMPLE);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");

    // Setup Dear ImGui style
    InitImGuiStyles();
}

Application::Application()
{
    InitializeWindow(this->window, this->gl_context, 1280, 720);
    renderer = std::make_unique<Renderer>();
    input = std::make_unique<Input>([this]() { this->running = false; });

}
Application::~Application()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::Run()
{
    uint32_t prevTime = SDL_GetTicks();
     
    while (running)
    {
        uint32_t currTime = SDL_GetTicks();
        float dt = static_cast<float>(currTime - prevTime) / 1000.f;
        prevTime = currTime;


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        
        input->Update();
        this->Update(dt);

        
        SDL_GetWindowSize(window, &screen_width, &screen_height);
        renderer->UpdateViewport(screen_width, screen_height);

        this->Render();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}