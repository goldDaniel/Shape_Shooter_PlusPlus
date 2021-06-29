#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <Core/Core.h>
#include <glad/glad.h>


class SpriteBatch
{

public:

   
    SpriteBatch();
    ~SpriteBatch();


    void Draw(Texture const* const tex,
        float const x, float const y,
        float const w, float const h);

    void Draw(Texture const* const tex,
        float const x, float const y,
        float const w, float const h,
        float rotation);

    
    void SetColor(const glm::vec4& color);

    void Begin(glm::mat4 proj, glm::mat4 view);
    void End();

private:

    bool has_begun = false;

    glm::mat4 proj = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);

    Texture const* current_texture = nullptr;

    glm::vec4 current_color = glm::vec4(1, 1, 1, 1);

    static const std::size_t MAX_SPRITES = 32;
    static const std::size_t POS_BUFFER_SIZE = MAX_SPRITES * 3 * 4;
    static const std::size_t COL_BUFFER_SIZE = MAX_SPRITES * 3 * 4;
    static const std::size_t TEX_BUFFER_SIZE = MAX_SPRITES * 2 * 4;

    static const std::size_t IDX_BUFFER_SIZE = MAX_SPRITES * 6;

    std::array<float, POS_BUFFER_SIZE> position_buffer;
    std::array<float, COL_BUFFER_SIZE> color_buffer;
    std::array<float, TEX_BUFFER_SIZE> texcoord_buffer;

    std::array<unsigned int, IDX_BUFFER_SIZE> index_buffer;
    unsigned int current_vertex_index = 0;

    std::size_t current_pos_index = 0;
    std::size_t current_col_index = 0;
    std::size_t current_tex_index = 0;
    std::size_t current_idx_index = 0;

    std::unique_ptr<Shader> shader;

    GLuint vertex_array;

    GLuint position_vbo;
    GLuint color_vbo;
    GLuint texcoord_vbo;

    GLuint index_vbo;

    /**
     * @brief Resets the buffers, this will cause additional calls to
     * overwrite the data currently held.
     *
     */
    void Clear();

    /**
     * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
     *
     */
    void Flush();

};

#endif

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H



class SpriteBatch
{
protected:

    


    /**
     * @brief Resets the buffers, this will cause additional calls to
     * overwrite the data currently held.
     *
     */
    virtual void Clear() = 0;

    /**
     * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
     *
     */
    virtual void Flush() = 0;

public:

    static std::unique_ptr<SpriteBatch> CreateSpriteBatch();

    virtual ~SpriteBatch() {}

    /**
     * @brief Sets the color for all following shapes.
     *
     * @param color The color the following shapes will be. Ignores alpha channel
     */
    void SetColor(const glm::vec4& color)
    {
        this->current_color = color;
    }

    void Begin(glm::mat4 proj, glm::mat4 view)
    {
        assert(!has_begun && "Spritebatch.end must be called before Spritebatch.begin");

        Clear();
        this->proj = proj;
        this->view = view;

        has_begun = true;
    }

    virtual void Draw(Texture const* const tex, float const x, float const y, float const w, float const h) = 0;
    virtual void Draw(Texture const* const tex, float const x, float const y, float const w, float const h, float rotation) = 0;

    void End()
    {
        assert(has_begun && "Spritebatch.end must be called before Spritebatch.begin");

        Flush();
        Clear();

        has_begun = false;
    }
};

#endif