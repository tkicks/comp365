CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include # 2011
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


helloworld:	helloworld.cpp
		g++ helloworld.cpp -o helloworld $(CFLAGS)

menu:		menu.cpp
		g++ menu.cpp -o menu $(CFLAGS)

square:		square.cpp
		g++ square.cpp -o square $(CFLAGS)

displayList:		displayList.cpp
		g++ displayList.cpp -o displayList $(CFLAGS)
		

clean:
		rm *.o