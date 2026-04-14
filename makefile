# --- DETECTION DU SYSTEME ---
ifeq ($(OS),Windows_NT)
    # Windows (w64devkit)
    CC = gcc
    EXT = .exe
    CFLAGS = -Iinclude
    LDFLAGS = lib/libraylib.a -lgdi32 -lwinmm -lopengl32
    RM = rm -f
else
    # macOS
    CC = gcc
    EXT =
    CFLAGS = -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
    RM = rm -f
endif

# --- FICHIERS ---
# Fichiers communs (utilisés par le jeu ET par les tests)
COMMON = ia.c moteur.c

# --- REGLES PRINCIPALES ---

# 1. Commande par défaut (compiler le jeu)
all: othello$(EXT)

# 2. Règle pour compiler le JEU (exclut tests.c)
othello$(EXT): main.c $(COMMON)
	$(CC) main.c $(COMMON) $(CFLAGS) $(LDFLAGS) -o othello$(EXT)

# 3. Règle pour compiler les TESTS (exclut main.c)
exec_tests$(EXT): tests.c $(COMMON)
	$(CC) tests.c $(COMMON) $(CFLAGS) $(LDFLAGS) -o exec_tests$(EXT)

# --- COMMANDES DE LANCEMENT ---

# Pour lancer le jeu : taper "make run"
run: othello$(EXT)
	./othello$(EXT)

# Pour lancer les tests : taper "make run-test"
run-test: exec_tests$(EXT)
	./exec_tests$(EXT)

# Pour tout supprimer : taper "make clean"
clean:
	$(RM) othello$(EXT) exec_tests$(EXT)