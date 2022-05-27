#pragma once
#include "Entity.h"

class Area : public Entity
{
public:
    Area();
    ~Area() = default;
    void Update();

public:
	int x = 200;
	int y = 200;
	int r = 100;
   
};
