//
// @description: Progress renderer class
// @author: von.wu
// @date: 2021-07-27 23:27:08
//

#ifndef PROGRESS_RENDERER_H
#define PROGRESS_RENDERER_H
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

    class ProgressRenderer
    {
    public:
        // Constructor (inits shaders/shapes)
        ProgressRenderer(Shader &shader);
        // Destructor
        ~ProgressRenderer();
        // Renders a defined quad textured with given sprite
        void DrawProgress(GLfloat timeLeft, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f);
        void SetBackgroundColor(glm::vec3 bgColor);
        void SetTimeLeftColor(glm::vec3 tlColor);

    private:
        // Render state
        Shader shader;
        GLuint quadVAO;
        // Initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
        glm::vec3 progressBGColor = glm::vec3(1.0f);
        glm::vec3 timeLeftColor = glm::vec3(0.5f, 0.5f, 0.5f);
    };
#ifdef __cplusplus
}
#endif
#endif // PROGRESS_RENDERER_H