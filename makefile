Debug_tb Debug_tb.c Debug2.h libDebug2.o:
	gcc -Wall -Wextra -I../Library -lDebug2 -g Debug_tb.c -o Debug_tb

libDebug2.o Debug2.h _Debug2.h Debug2.c:
	gcc -Wall -Wextra -I../Library -g -c Debug2.c -o libDebug2.o