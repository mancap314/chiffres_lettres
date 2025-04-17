#ifndef CHIFFRES_H_
#define CHIFFRES_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define N_OPS (4)

#define DELTA(a, b)  (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#define IS_USED(u, i) ((u) & (1 << (i)))
#define SET_USED(u, i) ((u) |= (1 << (i)))
#define SET_UNUSED(u, i)  ((u) &= (~(1 << (i))))

typedef struct {
    uint16_t operands[3];
    char op;
} step_t;

typedef struct {
    uint8_t n_max_steps;
    uint8_t n_steps;
    step_t *description;
} solution_t;

typedef struct {
    uint8_t n_solutions;
    uint8_t n_current;
    uint16_t delta;
    size_t n_combinations;
    size_t n_valid_combinations;
    solution_t **solutions;
} solution_set_t;


solution_set_t *newSolutionSet(uint8_t n_solutions, uint8_t n_max);
void delSolutionSet(solution_set_t *solution_set);

solution_t *newSolution(uint8_t n_max); 
void delSolution(solution_t *solution);

void print_array(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    uint8_t n_chiffres,
    FILE *f
);

void print_solution(
    solution_t *solution, 
    FILE *f
); 

void print_solution_set(
    solution_set_t *solution_set,
    FILE *f
);

uint16_t process_new_chiffre(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    solution_t *solution,
    uint8_t n_chiffres,
    uint32_t used_chiffres,
    uint16_t min_delta_reached,
    uint16_t target,
    uint8_t i,
    uint8_t j,
    uint8_t step_ind,
    char op
);


int process_chiffres(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    solution_t *solution,
    solution_set_t *solution_set,
    uint8_t n_chiffres,
    uint32_t used_chiffres,
    uint16_t target,
    uint16_t min_delta_reached,
    uint8_t step_ind
);

#endif
