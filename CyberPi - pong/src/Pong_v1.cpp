#include <Arduino.h>
#include "cyberpi.h"

CyberPi cyber;

#define S_W 127
#define S_H 127
#define P_W 4
#define P_H 16
#define BALL_SIZE 4

void draw_bitmap(int x, int y, int h, int w, uint16_t *image)
{

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      cyber.set_lcd_pixel(x + i, y + j, image[i * w + j]);
    }
  }
}

uint16_t w = 0xFFFF;

// DEFINIRANJE PADDLE LIJEVI:

int scoreA = 0;
int scoreB = 0;

uint16_t paddleImage[4][16] = {
    {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w},
    {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w},
    {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w},
    {w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w}};

uint16_t ballImage[4][4] = {
    {w, w, w, w},
    {w, w, w, w},
    {w, w, w, w},
    {w, w, w, w}};

// uint16_t loptica[]={}

// DEFINIRANJE PADDLE LIJEVI:
Sprite paddleL; // lijeva palica stvaranje lika - mi s njima napravimo
Sprite paddleR; // Oni samo prekopiraju kad uspiju paddleL
Sprite ball;    // mi s njima, oni fiziku analogija prema paddle
Sprite background = {
    0, 0, 4, 4, 4, 4}; // BONUS
// Sprite score1;     // mi s njima napravimo
// Sprite score2;     // kopiraju za drugog playera

void calculateMovement()
{
  if (cyber.get_button_a() && paddleR.y > 0)
  {
    paddleR.y -= 2;
  }
  if (cyber.get_button_b() && paddleR.y < S_H - 16)
  {
    paddleR.y += 2;
  }
  if (cyber.get_joystick_y() > 0 && paddleL.y < S_H - 16)
  {
    paddleL.y += 2;
  }
  if (cyber.get_joystick_y() < 0 && paddleL.y > 0)
  {
    paddleL.y -= 2;
  }
  ball.x = ball.xSpeed + ball.x;
  ball.y += ball.ySpeed;

  if (ball.y > S_H - BALL_SIZE || ball.y < 0)
  {
    ball.ySpeed = -ball.ySpeed;
    ball.y += ball.ySpeed;
  }

  if (ball.x + BALL_SIZE >= paddleR.x)
  {

    if (ball.y >= (paddleR.y) && ball.y <= (paddleR.y + paddleR.height))
    {
      ball.xSpeed *= -1;
    }
    else
    {
      scoreA++;
      ball.x = S_W / 4;
      ball.y = S_W / 2;
    }
  }

  if (ball.x < paddleL.x + P_W)
  {

    if (ball.y >= paddleL.y && ball.y <= paddleL.y + P_H)
    {
      ball.xSpeed *= -1;
    }
    else
    {
      scoreB++;
      ball.x = S_W / 4;
      ball.y = S_W / 2;
    }
  }

 
}

void draw()
{
  cyber.clean_lcd();
  // cyber.get_acc_x();
  cyber.set_bitmap(paddleL.x, paddleL.y, &paddleL);
  cyber.set_bitmap(paddleR.x, paddleR.y, &paddleR);
  cyber.set_bitmap(ball.x, ball.y, &ball);
  Sprite score1 = cyber.create_text(cyber.num_to_text(scoreA), 0xFFFF, 14);
  cyber.set_bitmap(50, 5, &score1);
  Sprite score2 = cyber.create_text(cyber.num_to_text(scoreB), 0xFFFF, 14);
  cyber.set_bitmap(70, 5, &score2);
  cyber.render_lcd();
}

void setup()
{

  cyber.begin();

  // Stvaranje likova:

  paddleL.width = P_W;
  paddleL.height = P_H;
  paddleL.buffer = (uint16_t *)paddleImage;
  paddleL.x = 10;
  paddleL.y = 67;

  paddleR.x = 118;
  paddleR.y = 67;
  paddleR.width = P_W;
  paddleR.height = P_H;
  paddleR.buffer = (uint16_t *)paddleImage;

  ball.xSpeed = 1;
  ball.ySpeed = 1;
  ball.x = S_W / 2;
  ball.y = S_H / 2;

  ball.width = BALL_SIZE;
  ball.height = BALL_SIZE;
  ball.buffer = (uint16_t *)ballImage;
}

void loop()
{
  calculateMovement();
  draw();
}
