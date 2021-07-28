
#include "rect_renderer.h"

RectRenderer::RectRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

RectRenderer::~RectRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void RectRenderer::DrawRect(glm::vec2 position, glm::vec2 size)
{
    // Prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f)); // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));   // Move origin of rotation to center of quad
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("rectColor", this->lineColor);
    // Render textured quad

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_LINE_LOOP, 0, 6);
    glBindVertexArray(0);
}

void RectRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        // 0.0f, 1.0f, 0.0f, 1.0f,
        // 1.0f, 1.0f, 1.0f, 1.0f,
        // 1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}