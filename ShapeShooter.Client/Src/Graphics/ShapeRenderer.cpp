#include "ShapeRenderer.h"

ShapeRenderer::ShapeRenderer()
{
    shader = std::make_unique<Shader>("Assets/Shaders/ShapeRendererVertex.glsl",
        "Assets/Shaders/ShapeRendererFragment.glsl");

    glGenVertexArrays(1, &vertex_array);

    glGenBuffers(1, &position_vbo);
    glGenBuffers(1, &color_vbo);

    current_color = glm::vec4(1.0);
}

ShapeRenderer::~ShapeRenderer()
{
    glDeleteBuffers(1, &position_vbo);
    glDeleteBuffers(1, &color_vbo);

    glDeleteVertexArrays(1, &vertex_array);
}

void ShapeRenderer::Begin(const glm::mat4& proj, const glm::mat4& view)
{
    assert(!has_begun && "ShapeRenderer->End() must be called before ShapeRenderer->Begin()");

    Clear();
    this->proj = proj;
    this->view = view;

    has_begun = true;
}

void ShapeRenderer::End()
{
    assert(has_begun && "ShapeRenderer->Begin() must be called before ShapeRenderer->End()");

    Flush();
    Clear();

    has_begun = false;
}

void ShapeRenderer::Clear()
{
    memset(&position_buffer[0], 0, position_buffer.max_size() * sizeof(float));
    memset(&color_buffer[0], 0, color_buffer.max_size() * sizeof(float));
    position_index = 0;
    color_index = 0;
}

void ShapeRenderer::Flush()
{
    if (position_index == 0) return;
    assert(has_begun && "ShapeRenderer must be running to flush data to GPU");

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * position_index, &position_buffer[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * color_index, &color_buffer[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    shader->Bind();
    shader->SetMat4("u_proj", proj);
    shader->SetMat4("u_view", view);

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(position_index / 3));
}

void ShapeRenderer::SetColor(const glm::vec4& color)
{
    current_color = color;
}


void ShapeRenderer::Line(const glm::vec2& p0, const glm::vec2& p1)
{
    if (position_index + 6 >= MAX_BUFFER_SIZE - 1)
    {
        Flush();
        Clear();
    }

    position_buffer[position_index + 0] = p0.x;
    position_buffer[position_index + 1] = p0.y;
    position_buffer[position_index + 2] = 0;
    position_buffer[position_index + 3] = p1.x;
    position_buffer[position_index + 4] = p1.y;
    position_buffer[position_index + 5] = 0;

    color_buffer[color_index + 0] = current_color.r;
    color_buffer[color_index + 1] = current_color.g;
    color_buffer[color_index + 2] = current_color.b;
    color_buffer[color_index + 3] = current_color.a;
    color_buffer[color_index + 4] = current_color.r;
    color_buffer[color_index + 5] = current_color.g;
    color_buffer[color_index + 6] = current_color.b;
    color_buffer[color_index + 7] = current_color.a;

    position_index += 6;
    color_index += 8;
}

void ShapeRenderer::Line(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& col0, const glm::vec4& col1)
{
    if (position_index + 6 >= MAX_BUFFER_SIZE - 1)
    {
        Flush();
        Clear();
    }

    position_buffer[position_index + 0] = p0.x;
    position_buffer[position_index + 1] = p0.y;
    position_buffer[position_index + 2] = 0;
    position_buffer[position_index + 3] = p1.x;
    position_buffer[position_index + 4] = p1.y;
    position_buffer[position_index + 5] = 0;

    color_buffer[color_index + 0] = col0.r;
    color_buffer[color_index + 1] = col0.g;
    color_buffer[color_index + 2] = col0.b;
    color_buffer[color_index + 3] = col0.a;
    color_buffer[color_index + 4] = col1.r;
    color_buffer[color_index + 5] = col1.g;
    color_buffer[color_index + 6] = col1.b;
    color_buffer[color_index + 7] = col1.a;

    position_index += 6;
    color_index += 8;
}

void ShapeRenderer::Circle(const glm::vec2& position, float radius, int segments)
{
    Ellipse(position, radius, radius, segments);
}

void ShapeRenderer::Ellipse(const glm::vec2& position, float rX, float rY, int segments)
{
    for (int i = 0; i < segments; i++)
    {
        float angle0 = (float)i / (float)segments * glm::pi<float>() * 2;
        float angle1 = (float)(i + 1) / (float)segments * glm::pi<float>() * 2;

        glm::vec2 p0 = position + glm::vec2(glm::cos(angle0) * rX, glm::sin(angle0) * rY);
        glm::vec2 p1 = position + glm::vec2(glm::cos(angle1) * rX, glm::sin(angle1) * rY);

        Line(p0, p1);
    }
}

void ShapeRenderer::Rect(const glm::vec2& min, const glm::vec2& max)
{
    glm::vec2 p0(min.x, min.y);
    glm::vec2 p1(max.x, min.y);
    glm::vec2 p2(max.x, max.y);
    glm::vec2 p3(min.x, max.y);

    Line(p0, p1);
    Line(p1, p2);
    Line(p2, p3);
    Line(p3, p0);
}