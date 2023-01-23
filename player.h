#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>

typedef struct PLAYER
{
private:
    uint8_t lives;
    uint8_t maxLives;
    uint8_t rockets;
    uint8_t maxRockets;
    bool shield;
    bool dead;

    uint8_t width;
    uint8_t height;

    int8_t posX;
    int8_t posY;

    int8_t bPosX;
    int8_t bPosY;
    bool bFired;
    bool bDead;
    uint8_t bMinSpeed;
    uint8_t bSpeed;
    uint8_t bMaxSpeed;

    uint8_t minSpeed;
    uint8_t speed;
    uint8_t maxSpeed;

public:
    void init();

    uint8_t getWidth();
    uint8_t getHeight();
    int8_t getX();
    int8_t getY();
    uint8_t getSpeed();

    void setX(uint8_t x);
    void moveLeft();
    void moveRight();

    void addSpeed(int8_t s);

    // bullet
    void shoot();
    void destroyBullet();
    int8_t getBulletX();
    int8_t getBulletY();
    void moveBullet();
    void addBulletSpeed(int8_t s);
    bool isBulletAlive();

    void addLife();
    void doDamage();
    void addRocket();
    void shootRocket();
    void addShield();

    uint8_t getLives();
    uint8_t getRockets();
    bool hasShield();
    bool isDead();

} PLAYER;

#endif