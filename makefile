CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include # 2011
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


helloworld:	helloworld.cpp
		g++ helloworld.cpp -o helloworld $(CFLAGS)

menu:		menu.cpp
		g++ menu.cpp -o menu $(CFLAGS)

parabola:		parabola.cpp
		g++ parabola.cpp -o parabola $(CFLAGS)
		
deCasteljau3:		deCasteljau3.cpp
		g++ deCasteljau3.cpp -o deCasteljau3 $(CFLAGS)

box:		box.cpp
		g++ box.cpp -o box $(CFLAGS)

clean:
		rm *.o
