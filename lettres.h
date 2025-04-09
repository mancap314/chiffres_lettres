#ifndef LETTRES_H_
#define LETTRES_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

#define MAX_PATH (256)
#define MAX_WORD_LENGTH (128)
#define MAX_N_MATCHES (5)


typedef struct {
    uint8_t n_matches;
    uint8_t length;
    size_t positions[MAX_N_MATCHES];
} word_matches_t;

size_t n_lines_in_file(const char fp[MAX_PATH]);

int load_dictionnaire(
    const char fp[MAX_PATH], 
    const size_t n_lines,
    wchar_t original_words[n_lines][MAX_WORD_LENGTH],
    char words[n_lines][MAX_WORD_LENGTH]
);

void swap(char *a, char *b);

void search_matches(
    uint8_t n_letters,
    char letters[n_letters],
    size_t n_words,
    char words[n_words][MAX_WORD_LENGTH],
    word_matches_t *word_matches
);

void search_best_matches(
    uint8_t n_letters,
    char letters[n_letters],
    size_t n_words,
    char words[n_words][MAX_WORD_LENGTH],
    word_matches_t *word_matches
);

#endif

