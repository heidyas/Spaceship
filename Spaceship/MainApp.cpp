// MainApp.cpp : Defines the entry point for the application.
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <sstream>
#include <cassert>
#include <iostream>

#include "MainApp.h"
#include "Animation.h"
#include "Area.h"
#include "Asteroid.h"
#include "Entity.h"
#include "Fire.h"
#include "Player.h"

using namespace sf;
using namespace std;

const float MainApp::DEGTORAD = 0.017453f;

bool IsCollide(Entity* a, Entity* b)
{
    bool isCollide = (b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y) < (a->R + b->R) * (a->R + b->R);
    return isCollide;
}

string IntToString(int a)
{
    ostringstream ost;
    ost << a;
    std::string s_num = ost.str();

    return s_num;
}
void Timer(int& sec)
{
    if (sec > 0) sec--;
    else return;
}

Text Menu(const Font& f, const String& string, int size, float x, float y) {
    Text text;
    text.setFont(f);
    text.setString(string);
    text.setCharacterSize(size);
    float width = text.getLocalBounds().width;
    text.setPosition(x - width / 2, y);
    return text;
}

int main()
{
    srand(time(0));

    bool play = false;
    bool gameOver = false;
    bool pause = false;
    bool inArea = false;
    bool win = false;

    //Timer and score
    int sec = MainApp::TIMER;
    Clock clock;
    int score = 0;
    int countArea = MainApp::COUNT_AREA;

    RenderWindow app(VideoMode(MainApp::W, MainApp::H), "Spaceship");
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4, t5, t6, t7, t8;
    t1.loadFromFile("assets/spaceship.png");
    t2.loadFromFile("assets/bg.jpg");
    t3.loadFromFile("assets/explosions/type_C.png");
    t4.loadFromFile("assets/rock2.png");
    t5.loadFromFile("assets/fire.png");
    t6.loadFromFile("assets/rock3.png");
    t7.loadFromFile("assets/explosions/type_B.png");
    t8.loadFromFile("assets/circle.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);

    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sFire(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_move(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
    Animation sArea(t8, 0, 0, 200, 200, 1, 0);
    
    std::list<Entity*> entities;

    //Add area in rand coordinate field
    Area* area = new Area();
    area->Settings(sArea, rand() % (MainApp::W - area->r), rand() % (MainApp::H - area->r), 0, area->r);
    entities.push_back(area);

    //Add spaceship
    Player* player = new Player();
    player->Settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(player);

    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Space)
                {
                    if (!play && !gameOver)
                        play = true;
                    else if (!play && gameOver)
                        play = true;
                		gameOver = false;
                }

            if (event.type == Event::MouseButtonPressed && play)
                if (event.mouseButton.button == Mouse::Left)
                {
                    Fire* fire = new Fire();
                    fire->Settings(sFire, player->x, player->y, player->angle, 10);
                    entities.push_back(fire);
                }
        }

        if (play)
        {
            if (Keyboard::isKeyPressed(Keyboard::D)) player->angle += 3;
            if (Keyboard::isKeyPressed(Keyboard::A))  player->angle -= 3;
            if (Keyboard::isKeyPressed(Keyboard::W)) player->move = true;
            else player->move = false;

            //Collide entities
            for (auto a : entities)
            {
                for (auto b : entities)
                {
                    if (a->name == "asteroid" && b->name == "fire")
                        if (IsCollide(a, b))
                        {
                            a->exist = false;
                            b->exist = false;

                            //Add +score
                            score += MainApp::SCORE;

                            Entity* entity = new Entity();
                            entity->Settings(sExplosion, a->x, a->y);
                            entity->name = "explosion";
                            entities.push_back(entity);

                            // Split asteroid into 2 parts
                            for (int i = 0; i < 2; i++)
                            {
                                if (a->R == 15) continue;
                                Entity* entity = new Asteroid();
                                entity->Settings(sRock_small, a->x, a->y, rand() % 360, 15);
                                entities.push_back(entity);
                            }
                        }

                    if (a->name == "player" && b->name == "asteroid")
                        if (IsCollide(a, b))
                        {
                            b->exist = false;
                            score = 0;

                            Entity* entity = new Entity();
                            entity->Settings(sExplosion_ship, a->x, a->y);
                            entity->name = "explosion";
                            entities.push_back(entity);

                            player->Settings(sPlayer, MainApp::W / 2, MainApp::H / 2, 0, 20);
                            player->dx = 0;
                            player->dy = 0;
                        }

                    //Timer in area
                    if (a->name == "player" && b->name == "area")
                        if (IsCollide(a, b) && clock.getElapsedTime().asMilliseconds() >= 999)
                        {
                            inArea = true;
                            Timer(sec);
                            clock.restart();
                            if (sec <= 0)
                            {
                                b->exist = false;
                                countArea--;
                                
                                //Add new area
                                if (countArea > 0)
                                {
                                    Area* area = new Area();
                                    area->Settings(sArea, rand() % (MainApp::W - area->r), rand() % (MainApp::H - area->r), 0, area->r);
									entities.push_back(area);

                                }
                                else win = true;
                               
                            }
                        }
                        else if (clock.getElapsedTime().asMilliseconds() >= 999)
                        {
                            sec = MainApp::TIMER;
                        }
                }
			}

            //Move anim spaceship
	        if (player->move)  player->anim = sPlayer_move;
	        else   player->anim = sPlayer;

            //End anim explosion
	        for (auto entity : entities)
	            if (entity->name == "explosion")
	                if (entity->anim.IsEnd()) entity->exist = false;

            //Add asteroids
	        if (rand() % 200 == 0 && inArea)
	        {
	            Asteroid* asteroid = new Asteroid();
	            asteroid->Settings(sRock, 0, rand() % MainApp::H, rand() % 360, 25);
	            entities.push_back(asteroid);
	        }

            //Update all entities
	        for (auto i = entities.begin(); i != entities.end();)
	        {
	            Entity* entity = *i;

	            entity->Update();
	            entity->anim.Update();

	            if (entity->exist == false) { i = entities.erase(i); delete entity; }
	            else ++i;
	        }
		}
        app.draw(background);

        //Text Menu
        Font font;
        if (!font.loadFromFile("assets/font/Datcub.ttf")) assert("Font not found");
        Text textStart = Menu(font, "Press Spacebar to Start", 45, MainApp::W / 2, MainApp::H / 3);
        Text textOver = Menu(font, "Game Over", 45, MainApp::W / 2, MainApp::H / 3);
        Text textPause = Menu(font, "Pause", 45, MainApp::W / 2, MainApp::H / 3);
        Text textTimer = Menu(font, "", 45, 0, 0);
        Text textScore = Menu(font, "", 45, MainApp::W - 300, 0);
        Text textWin = Menu(font, "You Win!", 45, MainApp::W / 2, MainApp::H / 3);

        //From int to string from string to Text
        textTimer.setString("Timer:" + IntToString(sec)); 
        textScore.setString("Score:" + IntToString(score));
        app.draw(textTimer);
        app.draw(textScore);

        if (!play) app.draw(textStart);
        if (gameOver) app.draw(textOver);
        if (pause) app.draw(textPause);
        if (win)
        {
            app.draw(textWin);
            entities.clear();
            app.display();
        }
        else
        {
            for (auto i : entities) i->Draw(app);
            app.display();
        }
    }
    return 0;
}
