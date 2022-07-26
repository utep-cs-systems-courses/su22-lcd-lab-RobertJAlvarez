#include <msp430.h>
#include <libTimer.h>
#include "screen.h"
#include "lcdutils.h"
#include "lcddraw.h"

char blue = 31, green = 0, red = 31;
unsigned char step = 0;

// axis zero for col, axis 1 for row

short drawPos[2] = {1,10}, controlPos[2] = {2, 10};
short colVelocity = 2, colLimits[2] = {1, screenWidth-3};  // -3 because that is the 'ball' size
short rowVelocity = 2, rowLimits[2] = {1, screenHeight-3}; // -3 because that is the 'ball' size

void draw_ball(short col, short row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}

void screen_update_ball()
{
  for (char axis = 0; axis < 2; axis++) 
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_BLUE);  /* erase */
  for (char axis = 0; axis < 2; axis++) 
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_WHITE); /* draw */
}

void screen_update_hourglass()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char lastStep = 0;

  if (step == 0 || (lastStep > step)) {
    clearScreen(COLOR_BLUE);
    lastStep = 0;
  } else {
    for (; lastStep <= step; lastStep++) {
      int startCol = col - lastStep;
      int endCol = col + lastStep;
      int width = 1 + endCol - startCol;

      // a color in this BGR encoding is BBBB BGGG GGGR RRRR
      unsigned int color = (blue << 11) | (green << 5) | red;

      fillRectangle(startCol, row+lastStep, width, 1, color);
      fillRectangle(startCol, row-lastStep, width, 1, color);
    }
  }
}

void update_shape()
{
  screen_update_ball();
  screen_update_hourglass();
}

