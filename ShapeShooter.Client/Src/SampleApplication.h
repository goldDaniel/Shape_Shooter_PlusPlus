#ifndef SAMPLE_APPLICATION_H_
#define SAMPLE_APPLICATION_H_

#include "Core/Application.h"

#include <Graphics/ShapeRenderer.h>
#include <Graphics/SpriteBatch.h>
#include <Util/Timer.h>
#include <SpringMass/SpringMassGrid.h>

static glm::vec2 ProjectToXY0Plane(const glm::vec2& mouse_pos,
	const glm::mat4& view,
	const glm::mat4& proj,
	const float window_width,
	const float window_height)
{
	const glm::vec4 viewport{ 0, window_height, window_width, -window_height };

	glm::vec3 screen_point_0(mouse_pos.x, mouse_pos.y, 0.f);
	glm::vec3 screen_point_1(mouse_pos.x, mouse_pos.y, 100.f);
	glm::vec3 model_point_0 = glm::unProject(screen_point_0, view, proj, viewport);
	glm::vec3 model_point_1 = glm::unProject(screen_point_1, view, proj, viewport);

	glm::vec3 plane_normal(0, 0, 1);
	glm::vec3 ray_direction = glm::normalize(model_point_1 - model_point_0);

	//Solve for d where dot((d * L + L0 - P0), n) = 0
	float d = glm::dot(-model_point_0, plane_normal) / glm::dot(ray_direction, plane_normal);

	//Use d to get back to point on plane
	glm::vec3 point_on_plane = model_point_0 + d * ray_direction;

	return glm::vec2(point_on_plane.x, point_on_plane.y);
}

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

public:
	SampleApplication() : Application()
	{
		s = std::make_unique<SpriteBatch>();
		sh = std::make_unique<ShapeRenderer>();
		renderer = std::make_unique<Renderer>();
		
		
		cam = Camera({0, 0, 30});
		grid = std::make_unique<SpringMassGrid>(glm::vec2{44, 22}, 0.1f);

		texture = renderer->CreateTexture2D("Assets/Textures/space.jpg");
	}

	~SampleApplication()
	{
	}

protected:
	virtual void Update(float dt) override
	{
		if (input->IsButtonJustPressed(SDL_BUTTON_LEFT))
		{
			auto screen_pos = input->GetMousePos();
			auto world_pos = ProjectToXY0Plane(	screen_pos, cam.GetViewMatrix(), 
												proj, 
												static_cast<float>(screen_width), 
												static_cast<float>(screen_height)
			);

			grid->ApplyRadialForce({ world_pos, -.05f }, 250.0f, 4.0f, {0.6f, 0.2f, 0.5f, 1.0f});
		}

		grid->Update(1.0f/60.0f);
	}

	virtual void Render() override
	{ 
		renderer->SetClearColor({0.1f, 0.01f, 0.1f, 1.0f});
		renderer->Clear();

		s->Begin(glm::mat4(1.0f), glm::mat4(1.0f));
		s->Draw(texture, 0, 0, 2, 2);
		s->End();

		proj = glm::perspective(glm::pi<float>() / 4.0f, (float)screen_width / (float)screen_height, 0.1f, 100.0f);
		sh->Begin(proj, cam.GetViewMatrix());
		grid->Render(sh.get());
		sh->End();
	}
};

#endif