OPT = -O3
WARNINGS = -Wall -Wextra
LIBRARY = -I../Library -L. -L../Library/libs
OPTIONS = -g
LIBS = -lDebug2

CC = gcc
CCFLAGS = $(WARNINGS) $(LIBRARY) $(LIBS) $(OPTIONS) $(OPT)

ifeq ($(OS),Windows_NT)
	EXEEXT = .exe
	DLLEXT = .dll
else
	EXEEXT = 
	DLLEXT = .o
endif

Debug_tb$(EXEEXT): Debug_tb.c Debug2.h libDebug2$(DLLEXT)
	$(CC) $(CCFLAGS) Debug_tb.c -o Debug_tb$(EXEEXT)

libDebug2$(DLLEXT): Debug2.h _Debug2.h Debug2.c
	$(CC) $(CCFLAGS) -c Debug2.c -o libDebug2$(DLLEXT)

clean:
	rm -f *.dll *.o *.exe *_tb