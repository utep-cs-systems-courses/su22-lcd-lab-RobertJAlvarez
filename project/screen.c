#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "screen.h"
#include "lcdutils.h"
#include "lcddraw.h"

//char blue = 31, green = 0, red = 31;

short drawPos[2] = {1,10}, controlPos[2] = {2, 10};        // axis zero for col, axis 1 for row
short colVelocity = 1, colLimits[2] = {1, screenWidth-3};  // -3 because that is the 'ball' size
short rowVelocity = 5, rowLimits[2] = {1, screenHeight-3}; // -3 because that is the 'ball' size

void drawMiddleDashLine()
{
  for (unsigned char colMin=0, rowMin=screenHeight/2; colMin < screenWidth; colMin+=4)
    fillRectangle(colMin, rowMin, 1, 1, COLOR_WHITE);
}

void draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}

short redrawScreen = 1;

void screen_update_ball()
{
  for (char axis = 0; axis < 2; axis++) 
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_BLACK);  /* erase */
  for (char axis = 0; axis < 2; axis++) 
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_WHITE); /* draw */
}

void update_shape()
{
  screen_update_ball();
}

void position_update_ball()
{
  {		/* move ball horizontally */
    short oldRow = controlPos[1];
    short newRow = oldRow + rowVelocity;
    if (newRow <= rowLimits[0] || newRow >= rowLimits[1])
	    rowVelocity = -rowVelocity;
    else
	    controlPos[1] = newRow;
  }
  {   /* move ball vertically */
    short oldCol = controlPos[0];
    short newCol = oldCol + colVelocity;
    if (newCol <= colLimits[0] || newCol >= colLimits[1])
	    colVelocity = -colVelocity;
    else
	    controlPos[0] = newCol;
  }
}

void wdt_c_handler()
{
  static int secCount = 0;

  if (++secCount >= 20) {		/* 12.5/sec */
    position_update_ball();
/*
    {				// update hourglass
      if (switches & SW3) green = (green + 1) % 64;
      if (switches & SW2) blue = (blue + 2) % 32;
      if (switches & SW1) red = (red - 3) % 32;
      if (step++ > 30)
        step = 0;
    }
    if (switches & SW4) return;
*/
    redrawScreen = 1;
    secCount = 0;
  }
}

