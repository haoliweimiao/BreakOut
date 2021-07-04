//
// @description: Sprite renderer class
// @author: von.wu
// @date: 2021-07-03 23:18:40
//

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
// include something
#include "single_import.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#ifdef __cplusplus
extern "C"
{
#endif
    // your code

    class SpriteRenderer
    {
    public:
        // Constructor (inits shaders/shapes)
        SpriteRenderer(Shader &shader);
        // Destructor
        ~SpriteRenderer();
        // Renders a defined quad textured with given sprite
        void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

    private:
        // Render state
        Shader shader;
        GLuint quadVAO;
        // Initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
    };
#ifdef __cplusplus
}
#endif
#endif // SPRITE_RENDERER_H