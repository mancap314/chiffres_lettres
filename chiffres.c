#include "chiffres.h"


static char list_ops[N_OPS] = {'+', '-', '*', '/'};

solution_set_t *newSolutionSet(uint8_t n_solutions, uint8_t n_max)
{
    solution_set_t *solution_set = malloc(sizeof(solution_set_t));
    if (NULL == solution_set)
        return NULL;

    solution_set->solutions = malloc(sizeof(solution_t) * n_solutions);
    if (NULL == solution_set->solutions) {
        free(solution_set);
        return NULL;
    }
    for (uint8_t i = 0; i < n_solutions; i++) {
        solution_set->solutions[i] = newSolution(n_max);
        if (NULL == solution_set->solutions[i]) {
            for (uint8_t j = 0; j < i; j++)
                delSolution(solution_set->solutions[j]);
            free(solution_set->solutions);
            free(solution_set);
        }
    }
    solution_set->n_solutions = n_solutions;
    solution_set->n_current = 0;
    solution_set->n_combinations = 0;
    solution_set->n_valid_combinations = 0;
    solution_set->delta = UINT16_MAX;
}

void delSolutionSet(solution_set_t *solution_set)
{
    for (uint8_t i = 0; i < solution_set->n_solutions; i++) 
        delSolution(solution_set->solutions[i]);
    free(solution_set->solutions);
    free(solution_set);
}

solution_t *newSolution(uint8_t n_max) 
{
    solution_t *solution = malloc(sizeof(solution_t));
    if (NULL == solution)
        return solution;

    solution->description = malloc(sizeof(step_t) * n_max);
    if (NULL == solution->description) {
        free(solution);
        return NULL;
    }
    solution->n_max_steps = n_max;
    solution->n_steps = 0;
}

void delSolution(solution_t *solution)
{
    if (NULL != solution) {
        free(solution->description);
        free(solution);
    }
}

void print_array(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    uint8_t n_chiffres,
    FILE *f
)
{
    if (n_chiffres > n_max)
        fprintf(stderr, "[ERROR] print_array(): can't print fir %u elements of array of max size %u\n",
                n_chiffres, n_max);

    for (uint8_t i = 0; i < n_chiffres; i++)
        fprintf(f, "%s%u%s", (i == 0) ? "[": "", chiffres[i], (i == n_chiffres - 1) ? "]\n": ", ");

    return;
}

void print_solution(
    solution_t *solution,
    FILE *f
) 
{
  if (solution->n_steps > solution->n_max_steps) {
    fprintf(stderr, "[ERROR] print_solution(): Can't print %u solution steps with n_max_steps = %u\n",
        solution->n_steps, solution->n_max_steps);
    return;
  }
  for (uint8_t i = 0; i < solution->n_steps + 1; i++)
    fprintf(f, "\t%u. %u %c %u = %u\n", 
        i + 1,
        solution->description[i].operands[0],
        solution->description[i].op,
        solution->description[i].operands[1],
        solution->description[i].operands[2]
    );
  return;
}

void print_solution_set(
    solution_set_t *solution_set,
    FILE *f
)
{
    fprintf(f, "- %zu combinations computed (from which %zu or %.2f%% are valid)\n"
            "- Min. delta to target reached: %u.\n"
            "- %u solutions:\n", 
            solution_set->n_combinations, solution_set->n_valid_combinations, 
            (float)solution_set->n_valid_combinations / (float)solution_set->n_combinations * 100.0f, 
            solution_set->delta - 2, solution_set->n_current);
    for (uint8_t i = 0; i < solution_set->n_current; i++) {
        fprintf(f, "solution %u:\n", i + 1);
        print_solution(solution_set->solutions[i], f);
        if (i < solution_set->n_current)
            fputs("===\n", f);
    }
}

void handle_solution(
    solution_set_t *solution_set, 
    solution_t *solution, 
    uint16_t delta
)
{
    if (delta < solution_set->delta 
            || (delta == solution_set->delta && solution->n_steps < solution_set->solutions[0]->n_steps)) {
        solution_set->delta = delta;
        solution_set->n_current = 0;
    }
    if (delta <= solution_set->delta 
            && (solution_set->n_current == 0 || solution->n_steps <= solution_set->solutions[0]->n_steps)
            && solution_set->n_current < solution_set->n_solutions) {
        // copy solution to solution_set->solutions
        solution_set->solutions[solution_set->n_current]->n_steps = solution->n_steps;
        for (uint8_t i = 0; i < solution->n_steps + 1; i++)
            memcpy(&solution_set->solutions[solution_set->n_current]->description[i], &solution->description[i], sizeof(solution_t));
        solution_set->n_current++;
    }
}

uint16_t process_new_chiffre (
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
)
{
   uint16_t new_chiffre = 0;
   int ret = 1;
   switch (op) {
     case '+':
       if (j > i && chiffres[j] > 0 && chiffres[i] > 0)
         new_chiffre = chiffres[i] + chiffres[j];
       break;
     case '-':
        if (chiffres[j] > 0 && chiffres[i] > chiffres[j])
         new_chiffre = chiffres[i] - chiffres[j];
       break;
     case '*':
        if (j > i && chiffres[j] > 1 && chiffres[i] > 1)
         new_chiffre = chiffres[i] * chiffres[j];
       break;
     case '/':
        if (chiffres[j] > 1 && chiffres[i] > 1 && chiffres[i] % chiffres[j] == 0)
         new_chiffre = chiffres[i] / chiffres[j];
       break;
     default:
       fprintf(stderr, "[ERROR] process_new_chiffre(): operation '%c' not valid.\n", op);
       ret = 0;
   }
   if (ret == 0)
     return 0;
   if (new_chiffre > 1) {
     solution->description[step_ind].operands[0] = chiffres[i];
     solution->description[step_ind].operands[1] = chiffres[j];
     solution->description[step_ind].operands[2] = new_chiffre;
     solution->description[step_ind].op = op;
     solution->n_steps = step_ind;
     chiffres[n_chiffres] = new_chiffre;
     min_delta_reached = DELTA(new_chiffre, target) + 2;
   } else 
     min_delta_reached = 1;

   return min_delta_reached;
}

int process_chiffres(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    solution_t *solution,
    solution_set_t * solution_set,
    uint8_t n_chiffres,
    uint32_t used_chiffres,
    uint16_t target,
    uint16_t min_delta_reached,
    uint8_t step_ind
)
{
  if (n_chiffres > n_max) {
    fprintf(stderr, "[ERROR] process_chiffres(): n_chiffres: %u > n_max: %u\n", n_chiffres, n_max);
    return 0;
  }
  uint8_t i, j, k;
  int ret;
  uint16_t new_min_delta_reached;
  for (i = 0; i < n_chiffres; i++) {
    for (j = 0; j < n_chiffres; j++) {
      if (i == j || IS_USED(used_chiffres, i) || IS_USED(used_chiffres, j))
        continue;
      SET_USED(used_chiffres, i);
      SET_USED(used_chiffres, j);
      for (k = 0; k < N_OPS; k++) {
        solution_set->n_combinations++;
        new_min_delta_reached = process_new_chiffre(
            n_max, 
            chiffres, 
            solution, 
            n_chiffres,
            used_chiffres, 
            min_delta_reached, 
            target, 
            i, 
            j, 
            step_ind, 
            list_ops[k]
        );
        if (new_min_delta_reached == 0) 
            return 0;
        if (new_min_delta_reached > 1) {
          solution_set->n_valid_combinations++;
          if (new_min_delta_reached < min_delta_reached 
                  || (new_min_delta_reached == min_delta_reached && solution->n_steps <= solution_set->solutions[0]->n_steps)) {
#ifdef VERBOSE
            fputs("[INFO] process_chiffres(): new best solution found:\n", stdout);
            print_solution(solution, stdout);
#endif
            min_delta_reached = new_min_delta_reached;
            handle_solution(solution_set, solution, min_delta_reached);
          }
          new_min_delta_reached = process_chiffres(
            n_max, 
            chiffres, 
            solution, 
            solution_set,
            n_chiffres + 1, 
            used_chiffres, 
            target, 
            min_delta_reached, 
            step_ind + 1
         );
         if (new_min_delta_reached == 0) 
            return 0;
         } 
      }  // end k-loop
      SET_UNUSED(used_chiffres, i);
      SET_UNUSED(used_chiffres, j);
    }  // end j-loop
  }  // end i-loop
  return min_delta_reached;
}

