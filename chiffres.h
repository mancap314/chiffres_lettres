#ifndef CHIFFRES_H_
#define CHIFFRES_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define N_NUMBERS (8)
#define MAX_NUMBER (100)  // Max value a number can take
#define SOL_SIZE (25)  // Number of character to describe a step
#define N_OPS (4)

#define DELTA(a, b)  (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#define IS_USED(u, i) ((u) & (1 << (i)))
#define SET_USED(u, i) ((u) |= (1 << (i)))
#define SET_UNUSED(u, i)  ((u) &= (~(1 << (i))))


void print_solution(
    uint8_t n_max, 
    char solution[n_max][SOL_SIZE], 
    uint8_t step_ind,
    FILE *f
); 


void print_solution(
    uint8_t n_max, 
    char solution[n_max][SOL_SIZE], 
    uint8_t step_ind,
    FILE *f
); 


int process_chiffres(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    char solution[n_max][SOL_SIZE];
    uint8_t n_chiffres,
    uint32_t used_chiffres,
    uint16_t target,
    uint16_t min_delta_reached,
    uint8_t step_ind
);

#endif
