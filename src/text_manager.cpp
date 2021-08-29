#include "text_manager.h"

static const int MAX_SHORT_VALUE = 65536;

void TextManager::Init(int width, int height)
{
    this->Width = width;
    this->Height = height;
    ResourceManager::LoadShader("shaders/text.vs", "shaders/text.frag", nullptr, "text");
    // Compile and setup the shader
    Shader shader = ResourceManager::GetShader("text");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    char path[256];
    getFilePath("fonts/arial.ttf", path);
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // 定义字体大小
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
    for (wchar_t c = 0; c < 128; c++)
    {
        // 加载字符的字形
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // 生成纹理
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x};
        Characters.insert(std::pair<wchar_t, Character>(c, character));
    }

    // 清理FreeType的资源
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextManager &TextManager::Use()
{
}

void TextManager::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    Shader shader = ResourceManager::GetShader("text");
    // 激活对应的渲染状态
    shader.Use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 遍历文本中所有的字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // printf("TextRenderSample::RenderText font:%c [xpos,ypos,w,h]=[%f, %f, %f, %f]", c, xpos, ypos, w, h);

        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},
            {xpos, ypos, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 1.0},

            {xpos, ypos + h, 0.0, 0.0},
            {xpos + w, ypos, 1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}};
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void LOGCATE(char *log)
{
    std::cout << log << std::endl;
}

void TextManager::LoadFacesByUnicode(const wchar_t *text, int size)
{
    // Compile and setup the shader
    Shader shader = ResourceManager::GetShader("text");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 0.0f, static_cast<GLfloat>(this->Height));
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        LOGCATE("TextRenderSample::LoadFacesByUnicode FREETYPE: Could not init FreeType Library");

    char path[256];
    getFilePath("fonts/msyh.ttc", path);

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
        LOGCATE("TextRenderSample::LoadFacesByUnicode FREETYPE: Failed to load font");

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    FT_Select_Charmap(face, ft_encoding_unicode);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (int i = 0; i < size; ++i)
    {
        // Character cache = Characters[text[i]];
        // if (cache.TextureID > 0)
        // {
        //     continue;
        // }

        if (FT_Load_Glyph(face, FT_Get_Char_Index(face, text[i]), FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
        {
            LOGCATE("TextRenderSample::LoadFacesByUnicode FREETYTPE: Failed to load Glyph");
            continue;
        }

        FT_Glyph glyph;
        FT_Get_Glyph(face->glyph, &glyph);

        //Convert the glyph to a bitmap.
        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

        //This reference will make accessing the bitmap easier
        FT_Bitmap &bitmap = bitmap_glyph->bitmap;

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            bitmap.width,
            bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bitmap.buffer);

        printf("TextRenderSample::LoadFacesByUnicode text[i]=%d [w,h,buffer]=[%d, %d, %p], advance.x=%ld \n", text[i], bitmap.width, bitmap.rows, bitmap.buffer, glyph->advance.x / MAX_SHORT_VALUE);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        // Character character = {
        //     texture,
        //     glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        //     glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        //     static_cast<GLuint>((glyph->advance.x / MAX_SHORT_VALUE) << 6)};
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x};
        Characters.insert(std::pair<wchar_t, Character>(text[i], character));
    }

    // 清理FreeType的资源
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextManager::RenderText(const wchar_t *text, int textLen, GLfloat x, GLfloat y, GLfloat scale,
                             glm::vec3 color)
{
    Shader shader = ResourceManager::GetShader("text");
    // 激活对应的渲染状态
    shader.Use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (int i = 0; i < textLen; ++i)
    {
        if (text[i] == 0)
        {
            continue;
        }
        Character ch = Characters[text[i]];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // printf("TextRenderSample::RenderText font:%d [xpos,ypos,w,h]=[%f, %f, %f, %f]\n", text[i], xpos, ypos, w, h);

        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},
            {xpos, ypos, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 1.0},

            {xpos, ypos + h, 0.0, 0.0},
            {xpos + w, ypos, 1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}};
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

char *textFileRead(char *filename)
{
    char *text;
    FILE *pf = fopen(filename, "r");
    fseek(pf, 0, SEEK_END);
    long lSize = ftell(pf);
    // 用完后需要将内存free掉
    text = (char *)malloc(lSize + 1);
    rewind(pf);
    fread(text, sizeof(char), lSize, pf);
    text[lSize] = '\0';
    return text;
}

void func_free(char **p)
{
    free(*p);
    *p = NULL;
    return;
}

const wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c) + 1;
    wchar_t *wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);

    return wc;
}

void TextManager::RenderTextFromFile(const char *file, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    char searchFile[256];
    bool checkFile = getFilePath(file, searchFile);
    if (!checkFile)
    {
        // 没找到对应的文本文件
        return;
    }

    // TextRenderSample::LoadFacesByUnicode text[i]=25105 [w,h,buffer]=[48, 45, 0000000015B788A0], advance.x=48
    // TextRenderSample::LoadFacesByUnicode text[i]=26159 [w,h,buffer]=[48, 44, 0000000015B7D710], advance.x=48
    // TextRenderSample::LoadFacesByUnicode text[i]=20320 [w,h,buffer]=[48, 46, 0000000015B7DF80], advance.x=48
    // TextRenderSample::LoadFacesByUnicode text[i]=29240 [w,h,buffer]=[48, 45, 0000000015B7E850], advance.x=48
    // TextRenderSample::LoadFacesByUnicode text[i]=29240 [w,h,buffer]=[48, 45, 0000000015B7F0F0], advance.x=48
    // wchar_t *WStr = L"我是你爸爸";
    // int wLen = wcslen(WStr) + 1;
    // LoadFacesByUnicode(WStr, wLen);
    // this->RenderText(WStr, wLen, x, y, scale, color);

    // Create and display a C style string, and then use it
    // to create different kinds of strings.
    const char *text = textFileRead(searchFile);
    printf("read wstring:%s \n", text);
    std::cout << text << " (char *)" << std::endl;

    // newsize describes the length of the
    // wchar_t string called wcstring in terms of the number
    // of wide characters, not the number of bytes.
    size_t newsize = strlen(text) + 1;

    // The following creates a buffer large enough to contain
    // the exact number of characters in the original string
    // in the new format. If you want to add more characters
    // to the end of the string, increase the value of newsize
    // to increase the size of the buffer.
    wchar_t *wcstring = new wchar_t[newsize];

    // Convert char* string to a wchar_t* string.
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wcstring, newsize, text, _TRUNCATE);
    // Display the result and indicate the type of string that it is.
    std::wcout << wcstring << " (wchar_t *) length : " << newsize << std::endl;

    LoadFacesByUnicode(wcstring, newsize);
    this->RenderText(wcstring, newsize, x, y, scale, color);
}