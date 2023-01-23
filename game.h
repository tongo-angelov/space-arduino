#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

typedef struct GAME
{
private:
    uint8_t Lives;
    uint8_t MaxLives;
    uint8_t Rockets;
    uint8_t MaxRockets;
    bool Shield;
    bool Dead;

    uint8_t Width;
    uint8_t Height;

    int8_t PosX;
    int8_t PosY;

    uint8_t MinSpeed;
    uint8_t Speed;
    uint8_t MaxSpeed;

public:
    void init();

    uint16_t Score = 0;
    uint16_t HighScore = 0;

    bool Paused = false;

    bool Beep = false;

    bool Motor = false;

    uint8_t DifficultyLimit = 200;
    uint8_t DifficultyTime = 1 * 1000;

} GAME;

#endif