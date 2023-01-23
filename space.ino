#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "sprites.h"
#include "game.h"
#include "player.h"

// Inputs
#define LEFT_BUTTON 4
#define SPECIAL_BUTTON 5
#define SHOOT_BUTTON 6
#define RIGHT_BUTTON 7
#define START_BUTTON 8

// Display
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 9
Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

// UI
#define UI_ZONE_SIZE 10
#define GAME_WIDTH 64
#define GAME_HEIGHT 118 // sreen size - ui zone size

// Motor
#define MOTOR 2
#define MOTOR_SHOOT_TIME 40
#define MOTOR_SPECIAL_TIME 100

// Sound
#define BUZZER 3
#define BEEP_SCORE_TIME 40
#define BEEP_SCORE_FREQ 1000
#define BEEP_DAMAGE_TIME 80
#define BEEP_DAMAGE_FREQ 500

// Battery
#define BATTERY_X 48
#define BATTERY_Y 2
#define LIPO A0
float lipoV = 0;
int battery_level = 0;

/// Time vars
unsigned long game_time;
unsigned long beep_time;
unsigned long motor_time;

/// Controls vars
int start_last = LOW;
int start_state = LOW;

int special_last = LOW;
int special_state = LOW;

int shoot_last = LOW;
int shoot_state = LOW;
bool shoot = false;

bool left = false;
bool right = false;

/// debug
unsigned int debug_frame_count = 0;
unsigned int fps = 0;

/// Game vars
GAME game;
PLAYER player;

void setup()
{
    Serial.begin(115200);

    analogReference(INTERNAL);

    pinMode(LEFT_BUTTON, INPUT_PULLUP);
    pinMode(START_BUTTON, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON, INPUT_PULLUP);
    pinMode(SHOOT_BUTTON, INPUT_PULLUP);
    pinMode(SPECIAL_BUTTON, INPUT_PULLUP);

    pinMode(MOTOR, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.setRotation(1);

    game_time = millis();
    init_game();
}

void init_game()
{
    player.init();
}

void loop()
{
    unsigned long _time = millis();
    if (_time > (game_time + 1000))
    {
        // Serial.print(" FPS: ");
        // Serial.print(debug_frame_count);
        // Serial.print('\n');
        fps = debug_frame_count;
        debug_frame_count = 0;
        game_time = _time;
    }

    if (game.Motor)
        if (_time > motor_time)
        {
            stopMotor();
        }

    if (game.Beep)
        if (_time > beep_time)
        {
            stopBeep();
        }

    read_battery();

    game_loop();

    debug_frame_count++;
}

void read_battery()
{
    lipoV = analogRead(LIPO) * (5.6 / 1023.00);

    if (lipoV > 4)
        battery_level = 4;
    else if (lipoV > 3.9)
        battery_level = 3;
    else if (lipoV > 3.8)
        battery_level = 2;
    else if (lipoV > 3.7)
        battery_level = 1;
    else
        battery_level = 0;
}

void game_loop()
{
    process_input();
    physics();
    reset_input();
    draw();
}

void process_input()
{
    start_state = digitalRead(START_BUTTON);

    if (start_state == LOW && start_last == HIGH)
    {
        pressed_start();
    }

    special_state = digitalRead(SPECIAL_BUTTON);

    if (special_state == LOW && special_last == HIGH)
    {
        pressed_special();
    }

    shoot_state = digitalRead(SHOOT_BUTTON);

    if (shoot_state == LOW && shoot_last == HIGH)
    {
        shoot = true;
    }

    left |= (digitalRead(LEFT_BUTTON) == LOW);
    right |= (digitalRead(RIGHT_BUTTON) == LOW);
}

void pressed_start()
{
    game.Paused = !game.Paused;
}

void pressed_special()
{
    player.addSpeed(1);

    startBeep(BEEP_DAMAGE_TIME, BEEP_DAMAGE_FREQ);
    startMotor(MOTOR_SHOOT_TIME);
}

void reset_input()
{
    left = false;
    right = false;
    start_last = start_state;
    shoot_last = shoot_state;
    special_last = special_state;
}

void physics()
{

    if (left && right)
    {
    }
    else if (left)
        player.moveLeft();
    else if (right)
        player.moveRight();

    if (player.getX() < 0)
        player.setX(0);
    else if (player.getX() > (SCREEN_WIDTH - player.getWidth()))
        player.setX(SCREEN_WIDTH - player.getWidth());

    if (player.isBulletAlive())
    {
        player.moveBullet();

        if (player.getBulletY() < 0)
            player.destroyBullet();
    }
    else
    {
        if (shoot)
        {
            startBeep(BEEP_SCORE_TIME, BEEP_SCORE_FREQ);
            startMotor(MOTOR_SHOOT_TIME);

            player.shoot();
            shoot = false;
        }
    }

    // calculateCollisions();
}

void draw()
{
    display.clearDisplay();
    if (game.Paused)
        drawPaused();
    drawUI();
    drawFrame();
    drawPlayer();
    if (player.isBulletAlive())
        drawBullet();
    if (player.hasShield())
        drawShield();
    display.display();
}

void drawDead()
{
    stopMotor();
    stopBeep();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(3, 60);
    display.println("DEAD:");
    display.setCursor(39, 60);
    display.println(game.HighScore);
    display.display();
}

void drawPaused()
{
    stopMotor();
    stopBeep();

    drawBattery();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(4, 60);
    display.println("PAUSED: ");
    display.setCursor(46, 60);
    display.println(game.HighScore);
}

void drawUI()
{
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if (battery_level <= 1)
        drawBattery();

    display.drawBitmap(3, 119, HEART_8x8, 8, 8, 1);
    display.setCursor(13, 120);
    display.println(player.getLives());

    display.setCursor(2, 2);
    display.println(fps);

    display.drawBitmap(47, 119, ROCKET_6x8, 6, 8, 1);
    display.setCursor(55, 120);
    display.println(player.getRockets());
}

void drawBattery()
{
    display.drawRect(BATTERY_X, BATTERY_Y, 15, 8, WHITE);
    int battery_indicator_x = BATTERY_X + 2;
    int battery_indicator_y = BATTERY_Y + 2;
    for (size_t i = 0; i < battery_level; i++)
    {
        display.drawBitmap(battery_indicator_x + i * 3, battery_indicator_y, BATTERY_2x4, 2, 4, 1);
    }
}

void drawFrame()
{
    // display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - UI_ZONE_SIZE + 1, WHITE);
    display.drawRect(0, SCREEN_HEIGHT - UI_ZONE_SIZE, SCREEN_WIDTH, UI_ZONE_SIZE, WHITE);
}

void drawPlayer()
{
    display.drawBitmap(player.getX(), player.getY(), PLAYER_11x11, 11, 11, 1);
}

void drawBullet()
{
    display.drawPixel(player.getBulletX(), player.getBulletY(), WHITE);
}

void drawShield()
{
    int ship_r = player.getWidth() / 2;
    display.drawCircle(player.getX() + ship_r, player.getY() + ship_r, ship_r + 1, 1);
}

void startMotor(int length)
{
    game.Motor = true;
    motor_time = millis() + length;
    digitalWrite(MOTOR, HIGH);
}
void stopMotor()
{
    game.Motor = false;
    digitalWrite(MOTOR, LOW);
}

void startBeep(int length, int freq)
{
    game.Beep = true;
    beep_time = millis() + length;
    tone(BUZZER, freq);
}

void stopBeep()
{
    game.Beep = false;
    noTone(BUZZER);
}
