OPT = -O3

include ../Library/make/InitMakefile

Debug_tb: Debug_tb.c Debug2.h | -lDebug2
	$(CC) $(CCFLAGS) -o $@ $< $|

include ../Library/make/DefaultMakefile
