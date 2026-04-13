CC = cc
CFLAGS = -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
SRC = main.c moteur.c interface.c ia.c
OBJ = othello

all:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OBJ)

clean:
	rm -f $(OBJ)