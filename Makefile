CC = gcc
CFLAGS = -O3
ifeq ($(OS),Windows_NT)
	EXT = exe
	RM = del
else 
	EXT = o
	RM = rm
endif

chiffres_lettres: chiffres_lettres.c chiffres.c lettres.c
	$(CC) -o $@.$(EXT) $^ $(CFLAGS)

test_chiffres: test_chiffres.c chiffres.c
	$(CC) -o $@.$(EXT) $^ $(CFLAGS)

test_lettres: test_lettres.c lettres.c
	$(CC) -o $@.$(EXT) $^ $(CFLAGS)

.PHONY: clean

clean:
	$(RM) *.$(EXT)

