#include "chiffres.h"


static char list_ops[N_OPS] = {'+', '-', '*', '/'};


void print_solution(
    uint8_t n_max, 
    char solution[n_max][SOL_SIZE], 
    uint8_t step_ind,
    FILE *f
) 
{
  if (step_ind - 1 >= n_max) {
    fprintf(stderr, "[ERROR] print_solution(): Can't print %u solution: n_max = %u\n",
        step_ind - 1, n_max);
    return;
  }
  for (uint8_t i = 0; i < step_ind - 1; i++)
    fprintf(f, "\t* %s\n", solution[i]);
  return;
}

uint16_t process_new_chiffre (
    uint8_t n_max,
    uint16_t chiffres[n_max],
    char solution[n_max][SOL_SIZE],
    uint32_t used_chiffres,
    uint16_t min_delta_reached,
    uint16_t target,
    uint8_t i,
    uint8_t j,
    uint8_t step_ind,
    char op
)
{
   SET_USED(used_chiffres, i);
   SET_USED(used_chiffres, j);
   uint16_t new_chiffre = 0;
   int ret = EXIT_SUCCESS;
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
       ret = EXIT_FAILURE;
   }
   if (ret == EXIT_FAILURE)
     return 0;
   if (new_chiffre > 0) {
     sprintf(solution[step_ind - 1], "%u / %u = %u", chiffres[i], chiffres[j], new_chiffre);
     chiffres[n_chiffres] = new_chiffre;
     if (DELTA(new_chiffre, target) < min_delta_reached)
       min_delta_reached = DELTA(new_chiffre, target) + 2;
   } else 
     min_delta_reached = 1;

   return min_delta_reached;
}

int process_chiffres(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    char solution[n_max][SOL_SIZE];
    uint8_t n_chiffres,
    uint32_t used_chiffres,
    uint16_t target,
    uint16_t min_delta_reached,
    uint8_t step_ind
)
{
  uint8_t i, j, k;
  int ret;
  step_ind++;
  for (i = 0; i < n_chiffres; i++) {
    for (j = 0; j < n_chiffres; j++) {
      if (i == j || IS_USED(used_chiffres, i) || IS_USED(used_chiffres, j))
        continue;
      for (k = 0; k < N_OPS; k++) {
        new_min_delta_reached = process_new_chiffre(n_max, chiffres, solution, used_chiffres, min_delta_reached, target, i, j, step_ind, list_ops[k]);
        if (new_min_delta_reached == 0) return EXIT_FAILURE;
        if (new_min_delta_reached > 1) {
          if (new_min_delta_reached < min_delta_reached) {
            fputs("[INFO] process_chiffres(): new best solution found:\n", stdout);
            print_solution(n_max, solution, step_ind, stdout);
          }
         ret = process_chiffres(n_max, chiffres, solution, n_chiffres + 1, used_chiffres, target, new_min_delta_reached, step_ind + 1);
         if (ret == EXIT_FAILURE) return EXIT_FAILURE;
        } 
      }  // end k-loop
      SET_UNUSED(used_chiffres, j);
    }  // end j-loop
    SET_UNUSED(used_chiffres, i);
  }  // end i-loop
  return EXIT_SUCCESS;
}
