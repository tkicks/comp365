CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include # 2011
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


circle:	circle.cpp
		g++ circle.cpp -o circle $(CFLAGS)

realcircle:	realcircle.cpp
		g++ realcircle.cpp -o realcircle $(CFLAGS)

colorfun:	colorfun.cpp
		g++ colorfun.cpp -o colorfun $(CFLAGS)

text:	text.cpp
		g++ text.cpp -o text $(CFLAGS)

font:	font.c
		g++ font.c -o font $(CFLAGS)

menu:	menu.cpp
		g++ menu.cpp -o menu $(CFLAGS)

mouse:	mouse.cpp
		g++ mouse.cpp -o mouse $(CFLAGS)

clean:
		rm *.o
