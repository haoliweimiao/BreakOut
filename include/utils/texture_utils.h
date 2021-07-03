//
// @description: Texture Utils
// @author: von.wu
// @date: 2021-06-27 16:25:13
//

#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

// include something
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "file_utils.h"
#include "stb/stb_include.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // your code
    GLuint createTexture(const char *path)
    {
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
                                                // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

        char imagePath[256];
        getFilePath(path, imagePath);
        if (imagePath == NULL)
        {
            std::cout << "load image file failed !" << std::endl;
            return -1;
        }

        unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        return textureId;
    }

#ifdef __cplusplus
}
#endif
#endif // TEXTURE_UTILS_H