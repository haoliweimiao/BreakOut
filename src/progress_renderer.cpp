
#include "progress_renderer.h"

ProgressRenderer::ProgressRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

ProgressRenderer::~ProgressRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void ProgressRenderer::SetBackgroundColor(glm::vec3 bgColor)
{
    this->progressBGColor = bgColor;
}
void ProgressRenderer::SetTimeLeftColor(glm::vec3 tlColor)
{
    this->timeLeftColor = tlColor;
}

void ProgressRenderer::DrawProgress(GLfloat timeLeft, glm::vec2 position, glm::vec2 size, GLfloat rotate)
{
    // Prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f)); // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));   // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));                // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    this->shader.SetMatrix4("model", model);

    // Render textured quad
    this->shader.SetVector3f("backColor", this->progressBGColor);
    this->shader.SetVector3f("timeColor", this->timeLeftColor);

    this->shader.SetFloat("timeLeft", timeLeft);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void ProgressRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

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