//
// @description: Text Manager
// @author: von.wu
// @date: 2021-07-25 18:35:31
//

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H
// include something
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <iostream>
#include <map>
#include <utils/file_utils.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "single_import.h"
#include "shader.h"
#include "resource_manager.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct Character
    {
        GLuint TextureID;   // 字形纹理的ID
        glm::ivec2 Size;    // 字形大小
        glm::ivec2 Bearing; // 从基准线到字形左部/顶部的偏移值
        GLuint Advance;     // 原点距下一个字形原点的距离
    };

    // your code
    class TextManager
    {
    public:
        // Constructor
        TextManager() {}
        void Init(int width, int height);
        // Sets the current shader as active
        TextManager &Use();
        std::map<GLchar, Character> Characters;
        GLuint VAO, VBO;
        void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
    };

#ifdef __cplusplus
}
#endif
#endif // TEXT_MANAGER_H