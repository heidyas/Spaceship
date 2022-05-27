#pragma once
#include "Entity.h"

class Asteroid : public Entity
{

public:
    Asteroid();
    ~Asteroid() = default;
    void Update();

};
