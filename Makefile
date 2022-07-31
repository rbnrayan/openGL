CC= gcc
CFLAGS= -Wall -Wextra
LIBS= -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lm

run: main
	./main

main: main.o glad.o
	$(CC) -o $@ $^ $(LIBS)

main.o: src/main.c
	$(CC) -c -o $@ $< $(LIBS)

glad.o: src/glad.c
	$(CC) -c -o $@ $< $(LIBS)

clean:
	rm -f main exo *.o
