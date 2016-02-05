CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include # 2011
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


kickhamG1:	kickhamG1.cpp
		g++ kickhamG1.cpp -o kickhamG1 $(CFLAGS)

clean:
		rm *.o
