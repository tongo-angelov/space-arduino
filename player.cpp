#include "player.h"

void PLAYER::init()
{
    this->lives = 3;
    this->maxLives = 3;
    this->rockets = 0;
    this->maxRockets = 5;
    this->shield = true;
    this->dead = false;

    this->width = 11;
    this->height = 11;

    this->posX = 10;
    this->posY = 105;

    this->bPosX = 10;
    this->bPosY = 105;
    this->bDead = true;
    this->bFired = false;
    this->bMinSpeed = 1;
    this->bSpeed = 10;
    this->bMaxSpeed = 20;

    this->minSpeed = 1;
    this->speed = 1;
    this->maxSpeed = 5;
}

uint8_t PLAYER::getWidth()
{
    return this->width;
}
uint8_t PLAYER::getHeight()
{
    return this->height;
}
int8_t PLAYER::getX()
{
    return this->posX;
}
int8_t PLAYER::getY()
{
    return this->posY;
}
uint8_t PLAYER::getSpeed()
{
    return this->speed;
}

void PLAYER::setX(uint8_t x)
{
    this->posX = x;
}

void PLAYER::moveLeft()
{
    this->posX -= this->speed;
}
void PLAYER::moveRight()
{
    this->posX += this->speed;
}
void PLAYER::addSpeed(int8_t s)
{

    if (s > 0)
    {
        if (this->speed + s > this->maxSpeed)
            this->speed = this->maxSpeed;
        else
            this->speed += s;
    }
    else
    {
        if (this->speed - s < this->minSpeed)
            this->speed = this->minSpeed;
        else
            this->speed -= s;
    }
}

/// bullet
void PLAYER::shoot()
{
    if (this->bDead)
    {
        if (!this->bFired)
        {
            this->bDead = false;
            this->bFired = true;
            this->bPosX = this->posX + this->width / 2;
            this->bPosY = this->posY;
        }
    }
}
void PLAYER::destroyBullet()
{
    this->bDead = true;
    this->bFired = false;
}
int8_t PLAYER::getBulletX()
{
    return this->bPosX;
}
int8_t PLAYER::getBulletY()
{
    return this->bPosY;
}
void PLAYER::moveBullet()
{
    this->bPosY -= this->bSpeed;
}
void PLAYER::addBulletSpeed(int8_t s)
{
    if (s > 0)
    {
        if (this->bSpeed + s > this->bMaxSpeed)
            this->bSpeed = this->bMaxSpeed;
        else
            this->bSpeed += s;
    }
    else
    {
        if (this->bSpeed - s < this->bMinSpeed)
            this->bSpeed = this->bMinSpeed;
        else
            this->bSpeed -= s;
    }
}
bool PLAYER::isBulletAlive()
{
    return !this->bDead;
}

void PLAYER::addLife()
{
    this->lives++;
}

void PLAYER::doDamage()
{
    if (this->shield)
        this->shield = false;
    else
    {
        this->lives--;
        if (
            this->lives <= 0)
            this->dead = true;
    }
}
void PLAYER::addRocket()
{
    this->rockets++;
}
void PLAYER::shootRocket()
{
    if (this->rockets > 0)
        this->rockets--;
}
void PLAYER::addShield()
{
    if (this->shield)
        if (this->lives < this->maxLives)
            this->lives++;
}
uint8_t PLAYER::getLives()
{
    return this->lives;
}
uint8_t PLAYER::getRockets()
{
    return this->rockets;
}

bool PLAYER::hasShield()
{
    return this->shield;
}

bool PLAYER::isDead()
{
    return this->dead;
}