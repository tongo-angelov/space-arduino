#include "game.h"

void GAME::init()
{
    this->Lives = 3;
    this->MaxLives = 3;
    this->Rockets = 0;
    this->MaxRockets = 5;
    this->Shield = true;
    this->Dead = false;

    this->Width = 11;
    this->Height = 11;

    this->PosX = 27;
    this->PosY = 105;

    this->MinSpeed = 64;
    this->Speed = 64;
    this->MaxSpeed = 200;
}