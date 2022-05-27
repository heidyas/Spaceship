#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Animation
{
public:
    Animation();
    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed);
	
    ~Animation() = default;

    void Update();
    bool IsEnd();
public:
    float frame, speed;
    Sprite sprite;
    std::vector<IntRect> frames;
};
