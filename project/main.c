#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "screen.h"
#include "lcdutils.h"
#include "lcddraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 
#define LED BIT6  /* note that bit zero req'd for display */

void main(void)
{
  P1DIR |= LED;   /**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();

  enableWDTInterrupts();    /**< enable periodic interrupt */
  or_sr(0x8);               /**< GIE (enable interrupts) */

  clearScreen(COLOR_BLACK);
  drawMiddleDashLine();
  while (1) {     /* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;  /* led off */
    or_sr(0x10);    /**< CPU OFF */
    P1OUT |= LED;   /* led on */
  }
}

