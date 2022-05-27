#pragma once
#include "Animation.h"

class Entity
{
public:

    Entity();
    virtual ~Entity() = default;

    void Settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1);
    virtual void Update() {}
    void Draw(RenderWindow& app);

public:
    float x, y, dx, dy, R, angle;
    bool exist;
    std::string name;
    Animation anim;

private:

};
