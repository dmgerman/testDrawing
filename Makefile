default: drawing



drawing:	drawing.c
	gcc -g drawing.c -o drawing `pkg-config --cflags --libs gtk+-3.0 gdk-3.0`


