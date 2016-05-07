CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include # 2011
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


Kickham_Gillis_G4:	Kickham_Gillis_G4.cpp
		g++ Kickham_Gillis_G4.cpp -o Kickham_Gillis_G4 $(CFLAGS)

clean:
		rm *.o
