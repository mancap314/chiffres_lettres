#include "chiffres_lettres.h"

int main(int argc, char *argv[argc + 1])
{
    char help[] = "Solver for the game 'les chiffres et les lettres'\n"
        "Options:\n"
        "\t [--chiffres | --lettres]: game to play (default: chiffres).\n"
        "\t -n --n-solutions: max. number of solutions computed (default: 5)\n"
        "\t -t --target: target number to reach (mandatory if playing chiffres, ignored otherwise\n"
        "\t -v --values: \n"
        "\t * if lettres: provide unaccented letters without separation\n"
        "\t * if chiffres: provide integer numbers separated by a character like ',' or ';'\n"
        "Examples:\n"
        "chiffres_lettres --chiffres --n-solutions 10 --target 356 --values 10,3,27,88,24,36,91\n"
        "chiffres_lettres --lettres --n-solutions 4 --values jorpebom\n"

    /* Set default values */
    uint8_t n_solutions = 5;

    /* parse arguments */
    for (uint8_t i = 0; i < argc; i++) {
        
    }
}

