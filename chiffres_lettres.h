#ifndef CHIFFRES_LETTRES_H_
#define CHIFFRES_LETTRES_H_

#include <ctype.h>
#include "chiffres.h"
#include "lettres.h"

#define N_MAX_CHIFFRES (15)
#define N_MAX_CHIFFRES_TOTAL (2 * N_MAX_CHIFFRES - 1)
#define N_MAX_CHARS_PER_CHIFFRE (5)
#define N_MAX_SOLUTIONS_CHIFFRES (10)
#define N_MAX_LETTRES (10)

typedef enum {
    NO_GAME = 0,
    CHIFFRES,
    LETTRES,
} game_e;

unsigned int str2uint(char *st);

#endif
