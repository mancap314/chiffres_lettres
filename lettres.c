#include "lettres.h"


size_t n_lines_in_file(const char fp[MAX_PATH])
{
    FILE *f = fopen(fp, "r");
    if (NULL == f) {
        fprintf(stderr, "[ERROR] n_lines_in_files(): Could not open file %s.\n", fp);
        exit(EXIT_FAILURE);
    }

    size_t n_lines = 0;
    char ch;
    while (!feof(f)) {
        ch = fgetc(f);
        if (ch == '\n') n_lines++;
    }
    fclose(f);
    return n_lines;
}

int load_dictionnaire(
    const char fp[MAX_PATH], 
    const size_t n_lines,
    wchar_t original_words[n_lines][MAX_WORD_LENGTH],
    char words[n_lines][MAX_WORD_LENGTH])
{
    setlocale(LC_CTYPE, "");
    FILE *f = fopen(fp, "r");
    if (NULL == f) {
        fprintf(stderr, "[ERROR] load_dictionnaire(): Could not open file %s.\n", fp);
        return EXIT_FAILURE;
    }
    size_t ind_line = 0;
    uint8_t ind_letter = 0, ind_letter_original = 0;
    const uint64_t minmaj_shift = (uint64_t)((wchar_t)'a' - (wchar_t)'A');
    wchar_t ch;
    while((ch = fgetwc(f)) != WEOF) {
        if (ch == L'\n') {
            original_words[ind_line][ind_letter_original] = L'\0';
            words[ind_line][ind_letter] = '\0';
            ind_line++;
            ind_letter = 0;
            ind_letter_original = 0;
            continue;
        }
        if (ind_letter_original >= MAX_WORD_LENGTH - 1)
            continue;

        original_words[ind_line][ind_letter_original] = ch;
        ind_letter_original++;
        if (ch >= L'a' && ch <= L'z') {
            words[ind_line][ind_letter] = (char)(ch - minmaj_shift);
            ind_letter++;
            continue;
        } else if (ch >= L'A' && ch <= L'Z') {
            words[ind_line][ind_letter] = (char)ch;
            ind_letter++;
            continue;
        } else {  // handle accentuated letters
                  // inspired by https://stackoverflow.com/questions/13238388/remove-letter-accents-from-a-given-text
            switch(ch) {
                case L'à': words[ind_line][ind_letter] = 'a'; ind_letter++; break;
                case L'â': words[ind_line][ind_letter] = 'a'; ind_letter++; break;
                case L'é': words[ind_line][ind_letter] = 'e'; ind_letter++; break;
                case L'è': words[ind_line][ind_letter] = 'e'; ind_letter++; break;
                case L'ê': words[ind_line][ind_letter] = 'e'; ind_letter++; break;
                case L'î': words[ind_line][ind_letter] = 'i'; ind_letter++; break;
                case L'ô': words[ind_line][ind_letter] = 'o'; ind_letter++; break;
                case L'ù': words[ind_line][ind_letter] = 'u'; ind_letter++; break;
                case L'û': words[ind_line][ind_letter] = 'u'; ind_letter++; break;
                default: break;
            }
        }
    }
    fclose(f);
    return EXIT_SUCCESS;
}

inline void swap(char *a, char *b) 
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

void search_matches(
    uint8_t n_letters,
    char letters[n_letters],
    size_t n_words,
    char words[n_words][MAX_WORD_LENGTH],
    word_matches_t *word_matches
)
{
    size_t low_ind = 0, mid_ind = n_words / 2, high_ind = n_words - 1;
    uint8_t n_matched_letters = 0;
    uint8_t i, j;
    int comp;
    char current_word_char;
    bool already_there;
    while(high_ind > low_ind + 1) {
        i = 0;
        comp = 0;
        current_word_char = words[mid_ind][i];
        while (comp == 0 && i < n_letters && current_word_char != '\0') {
            comp = letters[i] - current_word_char;
            if (comp == 0) 
                current_word_char = words[mid_ind][++i];
        }
        if (current_word_char == '\0') { // match found
            if (i > word_matches->length) {
#if DEBUG
                printf("[DEBUG] search_matches(): new best match found at position %zu: %u letters\n", mid_ind, i);
#endif
                word_matches->n_matches = 1;
                word_matches->length = i;
                word_matches->positions[0] = mid_ind;
            } else if (i == word_matches->length && word_matches->n_matches < MAX_N_MATCHES) {
                /* check that it is not already there */
                already_there = false;
                for (j = 0; j < word_matches->n_matches && !already_there; j++) {
                    already_there = (word_matches->positions[j] == mid_ind);
                }
                if (!already_there) {
#if DEBUG
                    printf("[DEBUG] search_matches(): new equally good match found at position %zu: %u letters\n", mid_ind, i);
#endif
                    word_matches->positions[word_matches->n_matches] = mid_ind;
                    word_matches->n_matches++;
                }
            }
        } 
        if (comp < 0) {
            high_ind = mid_ind;
            mid_ind -= (mid_ind - low_ind) / 2;
        } else {
            low_ind = mid_ind;
            mid_ind += (high_ind - mid_ind) / 2;
        }
    }
}

void search_best_matches(
    uint8_t n_letters,
    char letters[n_letters],
    size_t n_words,
    char words[n_words][MAX_WORD_LENGTH],
    word_matches_t *word_matches
)
{
    /* Computing all permutations of `letters` using non-recursive Heap algorithm */
    uint8_t *c = malloc(n_letters * sizeof(uint8_t));
    if (NULL == c) {
        fputs("[ERROR] search_best_matches(): Could not mallocate c\n", stderr);
        exit(EXIT_FAILURE);
    }
    memset(c, 0, n_letters * sizeof(uint8_t));

    search_matches(n_letters, letters, n_words, words, word_matches);

    uint8_t i = 0;
    while (i < n_letters) {
        if (c[i] < i) {
            i % 2 == 0 ? swap(&letters[0], &letters[i]): swap(&letters[c[i]], &letters[i]);            
            search_matches(n_letters, letters, n_words, words, word_matches);
            c[i]++;
            i = 1;
        } else { 
            c[i] = 0;
            i++;
        }
    }
    free(c);
}
