#include "Asteroid.h"
#include "MainApp.h"

Asteroid::Asteroid()
{
    dx = rand() % 8 - 4;
    dy = rand() % 8 - 4;
    name = "asteroid";
}

void Asteroid::Update()
{
    x += dx;
    y += dy;

    if (x > MainApp::W) x = 0;  if (x < 0) x = MainApp::W;
    if (y > MainApp::H) y = 0;  if (y < 0) y = MainApp::H;
}
