/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_object.h"
#include <iostream>

glm::vec3 genColor(int hardness)
{
    glm::vec3 color = glm::vec3(1.0f); // original: white
    // if (hardness = 1)
    //     color = glm::vec3(0.8f, 0.8f, 0.7f);
    // else
    if (hardness == 2)
        color = glm::vec3(0.2f, 0.6f, 1.0f);
    else if (hardness == 3)
        color = glm::vec3(0.0f, 0.7f, 0.0f);
    else if (hardness == 4)
        color = glm::vec3(0.8f, 0.8f, 0.4f);
    else if (hardness == 5)
        color = glm::vec3(1.0f, 0.5f, 0.0f);

    return color;
}

GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, int hardness)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), Hardness(hardness)
{
    this->Position = pos;
    this->Size = size;
    this->Sprite = sprite;
    this->Color = color;
    this->Velocity = velocity;
    this->Hardness = hardness;
}

void GameObject::Draw(SpriteRenderer &renderer)
{
    if (!this->IsSolid)
    {
        this->Color = genColor(this->Hardness);
    }
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}