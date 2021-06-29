#include "Application.h"
#include <Core/Core.h>


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
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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

        
        input->Update();
        this->Update(dt);

        
        SDL_GetWindowSize(window, &screen_width, &screen_height);
        renderer->UpdateViewport(screen_width, screen_height);

        this->Render();
        
        SDL_GL_SwapWindow(window);
    }
}