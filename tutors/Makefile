CLIBS=		-lm -lglut -lGLU -lGL -lX11
CPATHS=		-L/usr/lib -I/usr/include
CFLAGS=		$(CPATHS) $(CLIBS)


texture:	texture.c glm.c 
		gcc glm.c texture.c -o texture $(CFLAGS)

projection:	projection.c glm.c
		gcc glm.c projection.c -o projection $(CFLAGS)

clean:		*.o
		rm *.o
