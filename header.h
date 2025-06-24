#pragma once
#include <iostream>
#include<string>
#include<fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h> 
#include <windows.h> 

using namespace std;

// Abstract Base Class: GameObject
class GameObject {
protected:
    int x, y; // Position

public:
    GameObject(int x, int y);
    virtual void draw() = 0; // Abstract method for rendering
    virtual void move() = 0; // Abstract method for movement
    virtual ~GameObject();
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
};
// Player Class: Cheeseburger
class Cheeseburger : public GameObject {
private:
    int lives;
    int score;
    int speed;
    bool shieldActive;
    DWORD shieldActivationTime; // Time when the shield was activated (in milliseconds)
    DWORD shieldDuration; // 4 seconds (in milliseconds)
    bool multiplierActive;
    DWORD multiplierActivationTime;
    DWORD multiplierDuration;
public:
    Cheeseburger(int x, int y);
    void move(char direction);
    void activateMultiplier(int);
    void updateMultiplier();
    void activateShield(int);
    void gainScore(int points);
    void loseLife();
    void updateShield();
    int getLives() const;
    int getScore() const;
    void draw() override;
    void move() override;// Unused in this context
    void set();
};
// Obstacle Class: NyanCat
class NyanCat : public GameObject {
private:
    int fallSpeed;
public:
    NyanCat(int x, int y, int speed);
    void move() override;
    void setSpeed(int speed); // Adjust speed dynamically
    int getSpeed() const;
    void draw() override;

};
// PowerUp Class
class PowerUp : public GameObject {
private:
    string type;
public:
    PowerUp(int x, int y, string type);
    void draw() override;
    void move() override;
    string getType() const;

};
// Game Class
class Game {
private:
    Cheeseburger* player;
    NyanCat* nyanCats[5];
    PowerUp* powerUps[4];
    bool isGameOver;
    bool gameMode;
    int level;
    void render();
    void update();
    void reset();
    void saveScore();
    void displayScore();
public:
    Game();
    ~Game();
    void run();
    void Menu();
};
