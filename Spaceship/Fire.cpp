#include "Fire.h"
#include "MainApp.h"

Fire::Fire()
{
    name = "fire";
}

void Fire::Update()
{
    dx = cos(angle * MainApp::DEGTORAD) * 6;
    dy = sin(angle * MainApp::DEGTORAD) * 6;
    x += dx;
    y += dy;

    if (x > MainApp::W || x<0 || y>MainApp::H || y < 0) exist = false;
}
