default: drawing3 motion-snake



drawing2:	drawing.c
	gcc -g drawing.c -o drawing `pkg-config --cflags --libs gtk+-2.0 gdk-2.0`


drawing3:	drawing.c
	gcc -g drawing.c -o drawing3 `pkg-config --cflags --libs gtk+-3.0 gdk-3.0`


motion-snake: motion-snake.c
	gcc -o motion-snake motion-snake.c `pkg-config --cflags --libs gtk+-3.0`


