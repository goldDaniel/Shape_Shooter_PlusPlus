#ifndef SAMPLE_APPLICATION_H_
#define SAMPLE_APPLICATION_H_

#include "Core/Application.h"

#include <Graphics/ShapeRenderer.h>
#include <Graphics/SpriteBatch.h>
#include <Util/Util.h>
#include <Util/Timer.h>
#include <SpringMass/SpringMassGrid.h>


class SampleApplication : public Application
{
private:

	glm::mat4 proj;
	std::unique_ptr<SpringMassGrid> grid;


	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<ShapeRenderer> sh;

	std::unique_ptr<SpriteBatch> s;
	Texture const * texture;

	Camera cam;

	bool consistent_force = false;

	float force_amount = 250;
	float force_radius = 4;


public:
	SampleApplication() : Application()
	{
		s = std::make_unique<SpriteBatch>();
		sh = std::make_unique<ShapeRenderer>();
		renderer = std::make_unique<Renderer>();
		
		
		cam = Camera({0, 0, 50});
		cam.Front = -glm::normalize(cam.Position);
		grid = std::make_unique<SpringMassGrid>(glm::vec2{44, 22}, 0.5f);

		texture = renderer->CreateTexture2D("Assets/Textures/space.jpg");
	}

	~SampleApplication()
	{
	}

protected:
	virtual void Update(float dt) override
	{
		if (consistent_force)
		{
			grid->ApplyRadialForce({ 0,0, -.05f }, force_amount, force_radius);
		}

		grid->Update(dt);

		if (input->IsKeyDown(SDLK_r))
		{
			cam.Position = { 0, 0, 50 };
			cam.Front = -glm::normalize(cam.Position);
			cam.Up = { 0,1,0 };
		}

		if (input->IsKeyDown(SDLK_w))
		{
			cam.ProcessKeyboard(Camera_Movement::FORWARD, dt);
		}
		if (input->IsKeyDown(SDLK_s))
		{
			cam.ProcessKeyboard(Camera_Movement::BACKWARD, dt);
		}
		if (input->IsKeyDown(SDLK_a))
		{
			cam.ProcessKeyboard(Camera_Movement::LEFT, dt);
		}
		if (input->IsKeyDown(SDLK_d))
		{
			cam.ProcessKeyboard(Camera_Movement::RIGHT, dt);
		}

		static glm::vec2 prev = input->GetMousePos();
		if (input->IsButtonDown(SDL_BUTTON_LEFT))
		{
			glm::vec2 curr = input->GetMousePos();
			

			auto delta = curr - prev;
			cam.ProcessMouseMovement(delta.x, delta.y);
			prev = curr;
		}
		prev = input->GetMousePos();
	}

	virtual void Render() override
	{ 
		renderer->SetClearColor({0.1f, 0.01f, 0.1f, 1.0f});
		renderer->Clear();

		s->Begin(glm::mat4(1.0f), glm::mat4(1.0f));
		s->Draw(texture, 0, 0, 2, 2);
		s->End();

		proj = glm::perspective(glm::pi<float>() / 4.0f, (float)screen_width / (float)screen_height, 0.1f, 500.0f);
		sh->Begin(proj, cam.GetViewMatrix());
		grid->Render(sh.get());
		sh->End();


		ImGui::Begin("Grid Control", 0, ImGuiWindowFlags_NoMove);
		{
			bool reset_grid = false;
			float mass = 1.0f / grid->inverse_mass;

			float spacing = 1.0f / grid->spacing;

			if (ImGui::SliderFloat("density", &spacing, 0.01f, 10)) reset_grid = true;
			if (ImGui::SliderFloat("stiffness", &grid->stiffness, 0, 150)) reset_grid = true;
			if (ImGui::SliderFloat("damping", &grid->damping, 0, 50))	  reset_grid = true;	
			if (ImGui::SliderFloat("mass of point", &mass, 0.01f, 50.f))  reset_grid = true;
			grid->inverse_mass = 1.0f / mass;
			grid->spacing = 1.0f / spacing;

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::SliderFloat("force amount", &force_amount, 0, 1000)) reset_grid = true;
			if (ImGui::SliderFloat("force radius", &force_radius, 0, 50)) reset_grid = true;
			ImGui::Checkbox("Consistent Force", &consistent_force);
			
			if (reset_grid)
			{
				grid = std::make_unique<SpringMassGrid>(glm::vec2{ 44, 22 }, 
														grid->spacing, 
														grid->stiffness, 
														grid->damping, 
														grid->inverse_mass
				);
			}
		}
		ImGui::End();

	}
};

#endif