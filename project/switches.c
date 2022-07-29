#include <msp430.h>
#include <libTimer.h>
#include "switches.h"

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);  /* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES); /* if switch down, sense up */
  return p2val;
}

void switch_init()      /* setup switch */
{
  P2REN |= SWITCHES;    /* enables resistors for switches */
  P2IE |= SWITCHES;     /* enable interrupts from switches */
  P2OUT |= SWITCHES;    /* pull-ups for switches */
  P2DIR &= ~SWITCHES;   /* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;
void switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {       /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;         /* clear pending sw interrupts */
    switch_interrupt_handler(); /* single handler for all switches */
  }
}

