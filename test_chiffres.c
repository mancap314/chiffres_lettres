#include "chiffres.h"


int main(int argc, char *argv[argc - 1]) 
{
  srand(time(NULL));

  uint32_t target = 157;

  uint8_t n_chiffres = 6;
  uint8_t n_max =  2 * n_chiffres - 1;
  uint32_t used_chiffres = 0;
  uint8_t step_ind = 0;
  uint16_t min_delta_reached = UINT16_MAX;

  uint16_t *chiffres = malloc(sizeof(uint16_t) * n_max);
  if (NULL == chiffres) {
    fprintf(stderr, "[ERROR] main(): could not malloc chiffres with %u elements\n", n_max);
    exit(EXIT_FAILURE);
  }
  
  char (*solution)[SOL_SIZE] = malloc(sizeof(char[n_max][SOL_SIZE]));
  if (NULL == solution) {
    fprintf(stderr, "[ERROR] main(): could not malloc solution with %u * %u elements\n", n_max, SOL_SIZE);
    free(chiffres);
    exit(EXIT_FAILURE);
  }
  
  for (uint8_t i = 0; i < n_max; i++) 
    chiffres[i] = (i < n_chiffres) ? (uint16_t)((rand() % 100) + 1): 0;
  memset(solution, 0, sizeof(solution));

  print_array(n_max, chiffres, n_chiffres, stdout);

  int ret  = process_chiffres(n_max, chiffres, solution, n_chiffres, used_chiffres, target, min_delta_reached, step_ind);

  if (ret  > 0)
    fputs("SUCCESS\n", stdout);
  else 
    fputs("FAILURE\n", stderr);

  free(chiffres);
  free(solution);

  return EXIT_SUCCESS;
}

