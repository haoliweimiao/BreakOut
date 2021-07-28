//
// @description: Rect renderer class
// @author: von.wu
// @date: 2021-07-27 23:27:08
//

#ifndef RECT_RENDERER_H
#define RECT_RENDERER_H
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

    class RectRenderer
    {
    public:
        // Constructor (inits shaders/shapes)
        RectRenderer(Shader &shader);
        // Destructor
        ~RectRenderer();
        void DrawRect(glm::vec2 position, glm::vec2 size = glm::vec2(10, 10));

    private:
        // Render state
        Shader shader;
        GLuint quadVAO;
        // Initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
        glm::vec3 lineColor = glm::vec3(0.5f, 0.5f, 0.5f);
    };
#ifdef __cplusplus
}
#endif
#endif // RECT_RENDERER_H