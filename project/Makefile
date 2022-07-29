COMMON_OBJECTS  = screen.o switches.o wdt_handler.o main.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I./h
LDFLAGS         = -L./lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: proj.elf

libs:
	(cd timerLib && make install)
	(cd lcdLib && make install)

#additional rules for files
proj.elf: ${COMMON_OBJECTS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: proj.elf
	msp430loader.sh $^

clean:
	(cd timerLib && make clean)
	(cd lcdLib && make clean)
	rm -rf lib h
	rm -f *.o *.elf

switches.o: switches.c switches.h
screen.o: switches.h
main.o: screen.h switches.h

