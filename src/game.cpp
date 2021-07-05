/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"

// Game-related State data
SpriteRenderer *Renderer;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);

GameObject *Player;

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

BallObject *Ball;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, SHADER_NAME_SPRITE);
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader(SHADER_NAME_SPRITE).Use().SetInteger("image", 0);
    ResourceManager::GetShader(SHADER_NAME_SPRITE).SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, TEXTURE_NAME_FACE);
    ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
    // Set render-specific controls
    Shader shader = ResourceManager::GetShader(SHADER_NAME_SPRITE);
    Renderer = new SpriteRenderer(shader);

    // 加载纹理
    ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, TEXTURE_NAME_BACKGROUND);
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, TEXTURE_NAME_FACE);
    ResourceManager::LoadTexture("textures/block.png", GL_FALSE, TEXTURE_NAME_BLOCK);
    ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, TEXTURE_NAME_BLOCK_SOLID);
    // 加载关卡
    GameLevel one;
    one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
    GameLevel two;
    two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
    GameLevel three;
    three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
    GameLevel four;
    four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    // 加载挡板
    glm::vec2 playerPos = glm::vec2(
        this->Width / 2 - PLAYER_SIZE.x / 2,
        this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // 加载球
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                          ResourceManager::GetTexture(TEXTURE_NAME_FACE));
}

void Game::Update(GLfloat dt)
{
    // 更新对象
    Ball->Move(dt, this->Width);
    // 检测碰撞
    this->DoCollisions();
}

void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动玩家挡板
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
    }
}

void Game::Render()
{
    // Texture2D texture = ResourceManager::GetTexture(TEXTURE_NAME_FACE);
    // Renderer->DrawSprite(texture, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    if (this->State == GAME_ACTIVE)
    {
        // 绘制背景
        Renderer->DrawSprite(ResourceManager::GetTexture(TEXTURE_NAME_BACKGROUND),
                             glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        // 绘制关卡
        this->Levels[this->Level].Draw(*Renderer);

        // 绘制挡板
        Player->Draw(*Renderer);

        // 绘制球
        Ball->Draw(*Renderer);
    }
}

GLboolean CheckCollision(BallObject &one, GameObject &two)
{
    // GameObject &one, GameObject &two
    // // AABB - AABB collision
    // // x轴方向碰撞？
    // bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
    //                   two.Position.x + two.Size.x >= one.Position.x;
    // // y轴方向碰撞？
    // bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
    //                   two.Position.y + two.Size.y >= one.Position.y;
    // // 只有两个轴向都有碰撞时才碰撞
    // return collisionX && collisionY;

    // 获取圆的中心
    glm::vec2 center(one.Position + one.Radius);
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y);
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Ball, box))
            {
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
            }
        }
    }
}