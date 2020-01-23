hunter: hunter.c
	gcc hunter.c -lGL -lGLU -lglut -lm -o hunter 

.PHONY: clean

clean:
	-rm *.o 
