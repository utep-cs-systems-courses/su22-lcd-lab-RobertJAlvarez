#ifndef switches_included
#define switches_included

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15 // switches from 1-4

int switches;

void switch_init(); // setup switch
void wdt_c_handler();

#endif
