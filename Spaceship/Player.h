#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
   
    Player();
    ~Player() = default;
   
    void Update();

public:  
    bool move;
};
