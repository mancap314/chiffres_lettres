#include "chiffres_lettres.h"

unsigned int str2uint(char *st) {
    char *x;
    for (x = st ; *x ; x++) {
        if (!isdigit(*x))
            return 0L;
    }
    return (strtoul(st, 0L, 10));
}


int main(int argc, char *argv[argc + 1])
{
    char help_str[] = "Solver for the game 'les chiffres et les lettres'\n"
        "Options:\n"
        "\t [--chiffres | --lettres]: game to play (default: chiffres).\n"
        "\t -t --target: target number to reach (mandatory if playing chiffres, ignored otherwise\n"
        "\t -v --values: \n"
        "\t * if lettres: provide unaccented letters without separation (max. 10 letters, additional letters will be ignored)\n"
        "\t * if chiffres: provide integer numbers separated by a character like ',' or ';' (max. 15 chiffres of max. 5 digits, all above ignored)\n"
        "\t -h --help: display this help\n"
        "Examples:\n"
        "chiffres_lettres --chiffres --target 356 --values 10,3,27,88,24,36,91\n"
        "chiffres_lettres --lettres --values jorpebom\n";

    /* Set default values */
    bool target_set = false;
    game_e game = NO_GAME;
    uint8_t value_ind = 0;
    uint16_t target = 0;

    if (argc <= 2) {
        puts(help_str);
        exit(EXIT_SUCCESS);
    }

    /* parse arguments */
    for (uint8_t i = 1; i < argc; i++) {
        if (!strncmp(argv[i], "--chiffres", 11)) {
            game = CHIFFRES;
            continue;
        }
        if (!strncmp(argv[i], "--lettres", 10)) {
           game = LETTRES;
           continue;
        }
        if (!strncmp(argv[i], "--target", 9) 
                || !strncmp(argv[i], "-t", 3)) {
            target =  (uint16_t)str2uint(argv[++i]);
            if (target == 0) {
                fprintf(stderr, "Error: unvalid value %s provided for target argument. Must be a positive integer.\n", argv[i - 1]);
                exit(EXIT_FAILURE);
            }
        }
        if (!strncmp(argv[i], "--values", 9) 
                || !strncmp(argv[i], "-v", 3)) {
            value_ind = ++i;
        }
        if (!strncmp(argv[i], "--help", 7) 
                || !strncmp(argv[i], "-h", 3)) {
           puts(help_str); 
        }
    }

    /* Handle possible errors */
    if (game == NO_GAME) {
        fputs("Error: you must define the game with the option --chiffres or --lettres\n", stderr);
        exit(EXIT_FAILURE);
    }
    if (game == CHIFFRES && target == 0) {
        fputs("Error: when playing chiffres, you must define a strictly positive integer target with the option --target\n", stderr);
        exit(EXIT_FAILURE);
    }
    if (value_ind == 0) {
        fputs("Error: you must define values with the option --values\n", stderr);
        exit(EXIT_FAILURE);
    }

    /* Handle values */
    if (game == CHIFFRES) {
        uint16_t chiffres[N_MAX_CHIFFRES_TOTAL] = {0};
        char current_str[N_MAX_CHARS_PER_CHIFFRE] = {0};
        uint8_t char_ind = 0, n_chiffres = 0;
        for (char *x = argv[value_ind]; *x && n_chiffres < N_MAX_CHIFFRES - 1; x++) {
            if (isdigit(*x) && char_ind < N_MAX_CHARS_PER_CHIFFRE) {
                current_str[char_ind] = *x;
                char_ind++;
            } else if (char_ind > 0) {
                current_str[char_ind] = '\0';
                chiffres[n_chiffres] = strtoul(current_str, 0L, 10);
                n_chiffres++;
                char_ind = 0;
            }
        }
        if (char_ind > 0) {
            current_str[char_ind] = '\0';
            chiffres[n_chiffres] = strtoul(current_str, 0L, 10);
            n_chiffres++;
        }
        
        if (n_chiffres == 0) {
            fputs("Error: playing chiffres but no number has been provided in the --values argument\n", stderr);
            exit(EXIT_FAILURE);
        }
        printf("[INFO] Solving chiffres with following parameters:\n"
                "\t- chiffres: ");
        for (uint8_t i = 0; i < n_chiffres; i++)
            printf("%u%s", chiffres[i], i < n_chiffres - 1 ? ", ": "\n");
        printf("\t- target: %u\n", target);
        /* Compute and display solutions */
        uint8_t n_max = 2 * n_chiffres - 1;
        uint32_t used_chiffres = 0;
        uint8_t step_ind = 0;
        uint16_t min_delta_reached = UINT16_MAX;
        solution_t *solution = newSolution(n_max);
        if (NULL == solution) {
            fprintf(stderr, "[ERROR] main(): could not allocate solution.\n");
            exit(EXIT_FAILURE);
        }
        solution_set_t *solution_set = newSolutionSet(N_MAX_SOLUTIONS_CHIFFRES, n_max);
        if (NULL == solution_set) {
            fprintf(stderr, "[ERROR] main(): could not allocate solution_set.\n");
            free(solution);
            exit(EXIT_FAILURE);
        }
        clock_t start = clock();
        int ret = process_chiffres(n_max, chiffres, solution, solution_set, n_chiffres, used_chiffres, target, min_delta_reached, step_ind);
        clock_t end = clock();
        float duration = ((float)(end - start)) / (float)CLOCKS_PER_SEC;
        printf("[INFO]: All combinations computed for %u chiffres in %.2fs.\n", n_chiffres, duration); 

        if (ret  > 0) {
            fputs("SUCCESS\n", stdout);
            fputs("Best solution(s) found:\n", stdout);
            print_solution_set(solution_set, stdout);
        }
        else 
            fputs("FAILURE\n", stderr);

        delSolution(solution);
        delSolutionSet(solution_set);
    } else {  // process lettres
        char letters[N_MAX_LETTRES] = {0};
        uint8_t n_letters = 0;
        for (char *x = argv[value_ind]; *x && n_letters < N_MAX_LETTRES; x++) {
            if (*x >= 'A' && *x <= 'Z') {
                letters[n_letters] = *x;
                n_letters++;
            } else if (*x >= 'a' && *x <= 'z') {
                letters[n_letters] = *x - 'a' + 'A';
                n_letters++;
            }
        }
        printf("[INFO] Solving for lettres with following parameters:\n"
                "\t- lettres: ");
        for (uint8_t i = 0; i < n_letters; i++)
            printf("%c%s", letters[i], i < n_letters - 1 ? ", ": "\n");
        /* Compute and display solutions */
        /* 1. Load dictionnary and words */
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

        /* Search for solutions */
        start = clock();
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
    }
}

