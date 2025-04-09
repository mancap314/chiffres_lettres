#include "lettres.h"

int main(void) {

    clock_t start = clock();
    const char fp[MAX_PATH] = "liste.de.mots.francais.frgut.txt";
    size_t n_lines = n_lines_in_file(fp);
    printf("[INFO] main(): number of lines in %s: %zu\n", fp, n_lines);

    wchar_t (*original_words)[MAX_WORD_LENGTH] = malloc(sizeof(wchar_t[n_lines][MAX_WORD_LENGTH]));
    if (NULL == original_words) {
        fprintf(stderr, "[ERROR] Could not allocate original_words with %zu lines and max. %u letters per line.\n", n_lines, MAX_WORD_LENGTH);
        exit(EXIT_FAILURE);
    }

    char (*words)[MAX_WORD_LENGTH] = malloc(sizeof(char[n_lines][MAX_WORD_LENGTH]));
    if (NULL == words) {
        fprintf(stderr, "[ERROR] Could not allocate words with %zu lines and max. %u letters per line.\n", n_lines, MAX_WORD_LENGTH);
        exit(EXIT_FAILURE);
    }

    int ret = load_dictionnaire(fp, n_lines, original_words, words);
    clock_t end = clock();
    float duration = ((float)(end - start)) / (float)CLOCKS_PER_SEC;
    printf("[INFO] main(): %zu words loaded from dictionnaire file in %.2fs.\n", n_lines, duration); 
    
    /* Display first words */
    uint8_t n_words_displayed = 15;
    printf("[INFO] Displaying first %u original_word => word\n", n_words_displayed);
    for (uint8_t i = 0; i < n_words_displayed; i++) 
        printf("%ls => %s\n", original_words[i], words[i]);
    
    /* test search_best_matches */
    start = clock();
    char letters[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    uint8_t n_letters = 10;
    printf("Lettres: ");
    for (uint8_t i = 0; i < n_letters; i++)
        printf("%c%s", letters[i], i == n_letters - 1 ? "\n": ", ");

    word_matches_t word_matches = {0};
    word_matches.n_matches = 0;
    word_matches.length = 0;
    search_best_matches(n_letters, letters, n_lines, words, &word_matches);
    end = clock();
    duration = ((float)(end - start)) / (float)CLOCKS_PER_SEC;
    printf("[INFO] main(): matches for %u letters found in %.2fs.\n", n_letters, duration); 

    puts("matches:");
    for (uint8_t i = 0; i < word_matches.n_matches; i++)
        printf("\t* %ls\n", original_words[word_matches.positions[i]]);

    free(original_words);
    free(words);

    return EXIT_SUCCESS;
}
