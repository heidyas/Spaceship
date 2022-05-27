#include "Player.h"
#include "MainApp.h"

Player::Player()
{
    name = "player";
}

void Player::Update()
{
    if (move)
    {
        dx += cos(angle * MainApp::DEGTORAD) * 0.2;
        dy += sin(angle * MainApp::DEGTORAD) * 0.2;
    }
    else
    {
        dx *= 0.99;
        dy *= 0.99;
    }

    float speed = sqrt(dx * dx + dy * dy);
    if (speed > MainApp::MAX_SPEED)
    {
        dx *= MainApp::MAX_SPEED / speed;
        dy *= MainApp::MAX_SPEED / speed;
    }

    x += dx;
    y += dy;

    //do not let fly out of the screen size
    if (x > MainApp::W) x = MainApp::W; if (x < 0) x = 0;
    if (y > MainApp::H) y = MainApp::H; if (y < 0) y = 0;
}


