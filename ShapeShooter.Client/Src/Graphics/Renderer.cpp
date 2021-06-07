#include "Renderer.h"

#include <Core/Core.h>
#include "Renderable.h"


Renderer::Renderer()
{
	glClearColor(0, 0, 0, 1);

	glEnable(GL_BLEND);
}

Renderer::~Renderer()
{
	index_buffers.clear();
	vertex_arrays.clear();
	shaders.clear();
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::UpdateViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Renderer::Render(const Renderable& renderable) const
{
	if (renderable.render_backface)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
	
	renderable.shader->SetBool("render_wireframes", render_wireframes);

	//setup all our needed matrices
	renderable.shader->Bind();
	renderable.shader->SetMat4("model", renderable.model);
	renderable.shader->SetMat4("view", renderable.view);
	renderable.shader->SetMat4("projection", renderable.proj);

	//binds all textures in order they are added to the renderable
	for (uint32_t i = 0; i < renderable.textures.size(); i++)
	{
		renderable.shader->SetInt(renderable.texture_names[i], i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderable.textures[i]->ID);
	}

	glBindVertexArray(renderable.vArray->GetID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.iBuffer->GetID());

	uint32_t instances = renderable.vArray->num_instances;
	glDrawElementsInstanced(GL_TRIANGLES, renderable.iBuffer->GetNumElements(), GL_UNSIGNED_INT, 0, instances);
}


void Renderer::RenderWireframes(bool wireframes)
{
	render_wireframes = wireframes;
	if (wireframes)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


VertexArray* const Renderer::CreateVertexArray()
{
	auto resource = std::make_unique<VertexArray>();
	auto result = resource.get();

	vertex_arrays.push_back(std::move(resource));

	return result;
}

IndexBuffer* const Renderer::CreateIndexBuffer(const std::vector<unsigned int>& indices)
{
	auto resource = std::make_unique<IndexBuffer>(indices);
	auto result = resource.get();

	index_buffers.push_back(std::move(resource));

	return result;
}

Shader* const Renderer::CreateShader(const char* vertex_path, const char* frag_path)
{
	std::string key = vertex_path;
	key += frag_path;

	auto shaderIter = shaders.find(key);
	if (shaderIter != shaders.end())
	{
		return shaders[key].get();
	}

	shaders[key] = std::make_unique<Shader>(vertex_path, frag_path);

	return shaders[key].get();
}


Renderable* const Renderer::CreateRenderable(Shader* shader, VertexArray* v, IndexBuffer* i)
{
	for (auto& renderable : renderables)
	{
		if (renderable->shader == shader &&
			renderable->vArray == v &&
			renderable->iBuffer == i)
		{
			return renderable.get();
		}
	}

	auto resource = std::make_unique<Renderable>(shader, v, i);
	auto result = resource.get();

	renderables.push_back(std::move(resource));

	return result;
}

Texture* const Renderer::CreateTexture2D(const std::string& filepath)
{
	auto texturesIter = textures.find(filepath);
	if (texturesIter != textures.end())
	{
		return textures[filepath].get();
	}
	
	textures[filepath] = std::unique_ptr<Texture>(Texture::Create(filepath));

	return textures[filepath].get();
}
