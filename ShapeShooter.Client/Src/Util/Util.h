#ifndef UTIL_H
#define UTIL_H

#include <Core/Core.h>

namespace Util
{
	glm::vec2 ProjectToXY0Plane(const glm::vec2& mouse_pos,
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
}
#endif