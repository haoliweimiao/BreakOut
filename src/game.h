//
// @description: game class
// @author: von.wu
// @date: 2021-07-03 21:48:46
//

#ifndef GAME_H
#define GAME_H
// include something
#include "single_import.h"
#include "game_level.h"
#include <vector>

#define SHADER_NAME_SPRITE "sprite"
#define TEXTURE_NAME_FACE "face"
#define TEXTURE_NAME_BACKGROUND "background"
#define TEXTURE_NAME_BLOCK "block"
#define TEXTURE_NAME_BLOCK_SOLID "block_solid"

#ifdef __cplusplus
extern "C"
{
#endif

    // your code
    // 代表了游戏的当前状态
    enum GameState
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };

    class Game
    {
    public:
        // 游戏状态
        GameState State;
        GLboolean Keys[1024];
        GLuint Width, Height;
        std::vector<GameLevel> Levels;
        GLuint Level;
        // 构造函数/析构函数
        Game(GLuint width, GLuint height);
        ~Game();
        // 初始化游戏状态（加载所有的着色器/纹理/关卡）
        void Init();
        // 游戏循环
        void ProcessInput(GLfloat dt);
        void Update(GLfloat dt);
        void Render();
        void DoCollisions();
    };
#ifdef __cplusplus
}
#endif
#endif // GAME_H