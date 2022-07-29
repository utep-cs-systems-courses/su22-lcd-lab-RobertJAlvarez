/** \file lcdutils.h
 *  \brief Portions derived from EduKit code by RobG
 *  Created on: 10/19/2016
 *  Author: Eric Freudenthal & David Pruitt
 */

#ifndef lcdutils_included
#define lcdutils_included

typedef unsigned char u_char;
typedef unsigned int u_int;

extern const unsigned int font_11x16[95][11];

/** Orientation */
#define LONG_EDGE_PIXELS				        160
#define SHORT_EDGE_PIXELS				        128
#define ORIENTATION_VERTICAL			        0
#define ORIENTATION_HORIZONTAL			      1
#define ORIENTATION_VERTICAL_ROTATED	    2
#define ORIENTATION_HORIZONTAL_ROTATED	  3

/** Default Orientation */
#ifndef ORIENTATION		
#define ORIENTATION ORIENTATION_VERTICAL_ROTATED
#endif

#if (ORIENTATION == ORIENTATION_VERTICAL) || (ORIENTATION == ORIENTATION_VERTICAL_ROTATED)
# define screenWidth SHORT_EDGE_PIXELS
# define screenHeight LONG_EDGE_PIXELS
#else
# define screenHeight SHORT_EDGE_PIXELS
# define screenWidth LONG_EDGE_PIXELS
#endif

/** Initialize the onboard LCD */
void lcd_init();

/** Set area to draw to
 *  
 *  \param colStart Start column of the area
 *  \param rowStart Start row of the area
 *  \param colEnd End column of the area
 *  \param rowEnd End row of the area
 */
void lcd_setArea(u_char colStart, u_char rowStart, u_char colEnd, u_char rowEnd);

/** Write color to LCD
 *
 *  \param colorBGR The color in BGR
 */
void lcd_writeColor(u_int colorBGR);

#define rgb2bgr(val) ((((val) << 11)&0xf800) | ((val)&0x7e0) | (((val)>>11)&0x1f))

/** Colors */
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xffff

#endif /* lcdutils_included */
