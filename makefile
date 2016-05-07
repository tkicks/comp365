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

myanimate:		myanimate.cpp
		g++ myanimate.cpp -o myanimate $(CFLAGS)

moveTeapot:		moveTeapot.cpp
		g++ moveTeapot.cpp -o moveTeapot $(CFLAGS)

hermiteCubic:		hermiteCubic.cpp
		g++ hermiteCubic.cpp -o hermiteCubic $(CFLAGS)

<<<<<<< HEAD
square:		square.cpp
		g++ square.cpp -o square $(CFLAGS)

viewports:		viewports.cpp
		g++ viewports.cpp -o viewports $(CFLAGS)

squareOfWalls:		squareOfWalls.cpp
		g++ squareOfWalls.cpp -o squareOfWalls $(CFLAGS)

threeQuarterSphere:		threeQuarterSphere.cpp
		g++ threeQuarterSphere.cpp -o threeQuarterSphere $(CFLAGS)

sphereInBox1:		sphereInBox1.cpp
		g++ sphereInBox1.cpp -o sphereInBox1 $(CFLAGS)

lightAndMaterial1:		lightAndMaterial1.cpp
		g++ lightAndMaterial1.cpp -o lightAndMaterial1 $(CFLAGS)

lightAndMaterial2:		lightAndMaterial2.cpp
		g++ lightAndMaterial2.cpp -o lightAndMaterial2 $(CFLAGS)

sizeNormal:		sizeNormal.cpp
		g++ sizeNormal.cpp glm.c -o sizeNormal $(CFLAGS)

mytexture:		mytexture.cpp
		g++ mytexture.cpp glm.c -o mytexture $(CFLAGS)


=======
lightAndMaterial1:		lightAndMaterial1.cpp
		g++ lightAndMaterial1.cpp -o lightAndMaterial1 $(CFLAGS)

>>>>>>> 14acb4e52e1c44073385f20d95420f799318f53a
clean:
		rm *.o
