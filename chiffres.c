#include "chiffres.h"


static char list_ops[N_OPS] = {'+', '-', '*', '/'};


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
    uint8_t n_max, 
    char solution[n_max][SOL_SIZE], 
    uint8_t step_ind,
    FILE *f
) 
{
  if (step_ind > n_max) {
    fprintf(stderr, "[ERROR] print_solution(): Can't print %u solution: n_max = %u\n",
        step_ind, n_max);
    return;
  }
  for (uint8_t i = 0; i < step_ind + 1; i++)
    fprintf(f, "\t* %s\n", solution[i]);
  return;
}

uint16_t process_new_chiffre (
    uint8_t n_max,
    uint16_t chiffres[n_max],
    char solution[n_max][SOL_SIZE],
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
     sprintf(solution[step_ind], "%u %c %u = %u", chiffres[i], op, chiffres[j], new_chiffre);
     chiffres[n_chiffres] = new_chiffre;
     min_delta_reached = DELTA(new_chiffre, target) + 2;
   } else 
     min_delta_reached = 1;

   return min_delta_reached;
}

int process_chiffres(
    uint8_t n_max,
    uint16_t chiffres[n_max],
    char solution[n_max][SOL_SIZE],
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
          if (new_min_delta_reached <= min_delta_reached) {
            fputs("[INFO] process_chiffres(): new best solution found:\n", stdout);
            print_solution(n_max, solution, step_ind, stdout);
            min_delta_reached = new_min_delta_reached;
          }
          new_min_delta_reached = process_chiffres(
            n_max, 
            chiffres, 
            solution, 
            n_chiffres + 1, 
            used_chiffres, 
            target, 
            min_delta_reached, 
            step_ind + 1
         );
         if (new_min_delta_reached == 0) 
            return 0;
         } 
         if (new_min_delta_reached > 1 && new_min_delta_reached < min_delta_reached)
             min_delta_reached = new_min_delta_reached;
         
      }  // end k-loop
      SET_UNUSED(used_chiffres, i);
      SET_UNUSED(used_chiffres, j);
    }  // end j-loop
  }  // end i-loop
  return min_delta_reached;
}
