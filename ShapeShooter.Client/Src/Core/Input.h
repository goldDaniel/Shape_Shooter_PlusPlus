#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

#include <functional>
#include <unordered_map>

//need to send the SDL event to ImGui
#include <imgui/imgui_impl_sdl.h>

class Input
{
public:

    Input(const std::function<void()> functor)
    {
        quit_callback = functor;
    }

    bool IsKeyDown(SDL_KeyCode code) 
    {
        return key_down[code];
    }

    bool isKeyJustPressed(SDL_KeyCode code)
    {
        return key_down[code] && !key_down_previous[code];
    }

    bool IsButtonDown(int button)
    {
        return button_down[button];
    }

    bool IsButtonJustPressed(int button)
    {
        return button_down[button] && !button_down_previous[button];
    }
  
    //relative to top-left
    const glm::vec2 GetMousePos()
    {
        return { mouse_x, mouse_y };
    }

	void Update()
	{
        button_down_previous = button_down;
        key_down_previous = key_down;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit_callback();
                break;

            case SDL_KEYDOWN:
                key_down[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                key_down[event.key.keysym.sym] = false;
                break;

            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                button_down[event.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                button_down[event.button.button] = false;
                break;
            }
        }
	}

private:
   
    int mouse_x;
    int mouse_y;

    std::function<void()> quit_callback;

    std::unordered_map<int, bool> button_down;
    std::unordered_map<int, bool> button_down_previous;

    std::unordered_map<int, bool> key_down;
    std::unordered_map<int, bool> key_down_previous;
};

#endif