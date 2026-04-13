# 1. Détection de l'OS
OS := $(shell uname -s)

# 2. Fichiers communs
SRC = main.c moteur.c interface.c ia.c
TARGET = othello

# 3. Configuration selon l'OS
ifeq ($(OS), Darwin)
    # --- CONFIG MAC ---
    CC = cc
    CFLAGS = -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
else
    # --- CONFIG WINDOWS (w64devkit) ---
    CC = gcc
    # Ton ami doit vérifier si ce chemin est correct pour lui :
    RAYLIB_PATH = C:/raylib/raylib
    CFLAGS = -I$(RAYLIB_PATH)/src
    LDFLAGS = -L$(RAYLIB_PATH)/src -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = othello.exe
endif

# 4. Règles de compilation
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET) othello.exe