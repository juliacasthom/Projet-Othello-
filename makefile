# --- DETECTION DU SYSTEME ---
ifeq ($(OS),Windows_NT)
    # Configuration pour WINDOWS
    CC = gcc
    OBJ = othello.exe
    CFLAGS = -Iinclude
    LDFLAGS = -Llib -lraylib -lgdi32 -lwinmm -lopengl32
    RM = rm -f
else
    # Configuration pour MACOS
    CC = gcc
    OBJ = othello
    # On suppose que Raylib est installé via Homebrew sur Mac
    CFLAGS = -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
    RM = rm -f
endif

# --- FICHIERS ---
SRC = main.c moteur.c interface.c ia.c

# --- REGLES ---
all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OBJ)

clean:
	$(RM) $(OBJ)