#ifndef screen_included
#define screen_included

char blue, green, red;
unsigned char step;
short drawPos[2], controlPos[2];
short colVelocity, colLimits[2]; // -3 because that is the 'ball' size
short rowVelocity, rowLimits[2]; // -3 because that is the 'ball' size

void draw_ball(short col, short row, unsigned short color);
void update_shape();

#endif
