#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "screen.h"
#include "lcdutils.h"
#include "lcddraw.h"

void drawMiddleDashLine()
{
  for (unsigned char colMin=0, rowMin=screenHeight/2; colMin < screenWidth; colMin+=4)
    fillRectangle(colMin, rowMin, 1, 1, COLOR_WHITE);
}

void draw_bar(int col, int row, unsigned short color)
{
  fillRectangle(col, row, 11, 1, color);
}

short barDrawPos[2] = {screenWidth/2, screenWidth/2}; // left bar on 0, right bar on 1
short barControlPos[2] = {1+screenWidth/2, 1+screenWidth/2};
short barLim[2] = {5,screenWidth-8};  //bar is of size 5 and bar have shifts of 3

void screen_update_bar()
{
  for (char bar = 0; bar < 2; bar++) 
    if (barDrawPos[bar] != barControlPos[bar]) // position changed?
      goto redraw;
  return;			// nothing to do
 redraw:
  draw_bar(barDrawPos[0]-5, 2, COLOR_BLACK);  // erase left bar
  draw_bar(barDrawPos[1]-5, screenHeight-2, COLOR_BLACK);  // erase right bar
  for (char bar = 0; bar < 2; bar++) 
    barDrawPos[bar] = barControlPos[bar];
  draw_bar(barDrawPos[0]-5, 2, COLOR_WHITE); // draw left bar
  draw_bar(barDrawPos[1]-5, screenHeight-2, COLOR_WHITE); // draw right bar
}

void position_update_bar()
{
  if (switches & SW1 && barControlPos[1] > barLim[0]) barControlPos[1] -= 3;
  if (switches & SW2 && barControlPos[1] < barLim[1]) barControlPos[1] += 3;
  if (switches & SW3 && barControlPos[0] > barLim[0]) barControlPos[0] -= 3;
  if (switches & SW4 && barControlPos[0] < barLim[1]) barControlPos[0] += 3;
}

void draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}

short ballDrawPos[2] = {screenWidth/2,screenHeight/2};         // Axis 0 for col, axis 1 for row
short ballControlPos[2] = {1+screenWidth/2, 1+screenHeight};   // Axis 0 for col, axis 1 for row
short ballColVelocity = 1, ballColLim[2] = {1, screenWidth-3}; // -3 because of ball size
short ballRowVelocity = 4, ballRowLim[2] = {1, screenHeight-3};// -3 because of ball size

void screen_update_ball()
{
  for (char axis = 0; axis < 2; axis++) 
    if (ballDrawPos[axis] != ballControlPos[axis]) // position changed?
      goto redraw;
  return;			// nothing to do
 redraw:
  draw_ball(ballDrawPos[0], ballDrawPos[1], COLOR_BLACK);  // erase
  for (char axis = 0; axis < 2; axis++) 
    ballDrawPos[axis] = ballControlPos[axis];
  draw_ball(ballDrawPos[0], ballDrawPos[1], COLOR_WHITE); // draw
}

void bounce_ball(char at_bar)
{
  //If ball doesn't hit the center of the bar, make the ball move two pixels faster to that direction where the ball hit the bar
  if (ballControlPos[0]+1 < barControlPos[at_bar] && ballColVelocity > -3)
    ballColVelocity -= 1;
  else if (ballControlPos[0]+1 > barControlPos[at_bar] && ballColVelocity < 3)
    ballColVelocity += 1;
  ballRowVelocity  = -ballRowVelocity;
}

char score[2] = {'0','0'};
char prev_score[2] = {'1','1'};

void position_update_ball()
{
  {   // move ball horizontally
    short oldCol = ballControlPos[0];
    short newCol = oldCol + ballColVelocity;
    if (newCol <= ballColLim[0] || newCol >= ballColLim[1])
      ballColVelocity = -ballColVelocity;
    else
      ballControlPos[0] = newCol;
  }

  {		// move ball vertically
    short oldRow = ballControlPos[1];
    short newRow = oldRow + ballRowVelocity;
    if (newRow <= ballRowLim[0]) {      // ball in upper row screen edge
      if (ballControlPos[0]+2 < barControlPos[0]-5 || ballControlPos[0] > barControlPos[0]+8)
        score[1]++;
      else
        bounce_ball(0);
    }
    else if (newRow >= ballRowLim[1]) {  // ball in lower row screen edge
      if (ballControlPos[0]+2 < barControlPos[1]-5 || ballControlPos[0] > barControlPos[1]+8)
        score[0]++;
      else
        bounce_ball(1);
    }
    else
      ballControlPos[1] = newRow;
  }
}

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;
  if (++secCount >= 20) {		// 12.5/sec
    position_update_ball();
    position_update_bar();
    redrawScreen = 1;
    secCount = 0;
  }
}

void restart_game()
{
  drawMiddleDashLine();

  barControlPos[0] = 1+screenWidth/2;   //Center upper bar
  barControlPos[1] = 1+screenWidth/2;   //Center lower bar
  ballControlPos[1] = 1+screenHeight/2; //Center ball vertically (row wise)

  redrawScreen = 1;
}

void draw_score()
{
  drawChar11x16(10, screenHeight/4, score[0], COLOR_WHITE, COLOR_BLACK);
  drawChar11x16(10, (3*screenHeight)/4, score[1], COLOR_WHITE, COLOR_BLACK);
}

void screen_update_score()
{
  for (char i=0; i<2; i++)
    if (prev_score[i] != score[i]) {
      restart_game();
      prev_score[i] = score[i];
      draw_score();
      break;
    }
}

void update_shape()
{
  screen_update_score();
  if (score[0] > '5' || score[1] > '5') {
    score[0] = '0';
    score[1] = '0';
  }
  screen_update_bar();
  screen_update_ball();
}

