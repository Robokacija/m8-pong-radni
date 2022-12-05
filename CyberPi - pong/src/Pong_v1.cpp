#include <Arduino.h>
#include "cyberpi.h"

CyberPi cyber;

#define S_W 127
#define S_H 127
#define P_W 4
#define P_H 16
#define BALL_SIZE 4

uint16_t w = 0xFFFF;


uint16_t ballImage[4][4] = {
    {w, w, w, w},
    {w, w, w, w},
    {w, w, w, w},
    {w, w, w, w}};


Sprite ball;    // mi s njima, oni fiziku analogija prema paddle

void calculateMovement()
{
  ball.x = ball.x + ball.xSpeed;
  ball.y += ball.ySpeed;

  if (ball.y > S_H - BALL_SIZE || ball.y < 0)
  {
    ball.ySpeed = -ball.ySpeed;
    ball.y += ball.ySpeed;
  }
    if (ball.x > S_W - BALL_SIZE || ball.x < 0)
  {
    ball.xSpeed = -ball.xSpeed;
    ball.x += ball.xSpeed;
  }

}

void draw()
{
  cyber.clean_lcd();
  cyber.set_bitmap(ball.x, ball.y, &ball);
  cyber.render_lcd();
}

void setup()
{

  cyber.begin();

  // Stvaranje likova:

  ball.xSpeed = 1;
  ball.ySpeed = 1;
  ball.x = S_W / 2;
  ball.y = S_H / 4;
  ball.width = BALL_SIZE;
  ball.height = BALL_SIZE;
  ball.buffer = (uint16_t *)ballImage;
}

void loop()
{
  calculateMovement();
  draw();
}
