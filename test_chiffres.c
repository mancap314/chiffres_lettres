#include "chiffres.h"


int main(int argc, char *argv[argc - 1]) 
{
  srand(time(NULL));

  uint32_t target = 157;

  uint8_t n_solutions = 10;  // max number of solutions to keep for the best delta reached
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
  
  for (uint8_t i = 0; i < n_max; i++) 
    chiffres[i] = (i < n_chiffres) ? (uint16_t)((rand() % 100) + 1): 0;

  solution_t *solution = newSolution(n_max);
  if (NULL == solution) {
      fprintf(stderr, "[ERROR] main(): could not allocate solution.\n");
      free(chiffres);
      exit(EXIT_FAILURE);
  }

  solution_set_t *solution_set = newSolutionSet(n_solutions, n_max);
  if (NULL == solution_set) {
      fprintf(stderr, "[ERROR] main(): could not allocate solution_set.\n");
      free(chiffres);
      free(solution);
      exit(EXIT_FAILURE);
  }

  print_array(n_max, chiffres, n_chiffres, stdout);

  clock_t start = clock();
  int ret = process_chiffres(n_max, chiffres, solution, solution_set, n_chiffres, used_chiffres, target, min_delta_reached, step_ind);
  clock_t end = clock();
  float duration = ((float)(end - start)) / (float)CLOCKS_PER_SEC;
  printf("[INFO] main(): All combinations computed for %u chiffres in %.2fs.\n", n_chiffres, duration); 

  if (ret  > 0) {
    fputs("SUCCESS\n", stdout);
    fputs("Best solution(s) found:\n", stdout);
    print_solution_set(solution_set, stdout);
}
  else 
    fputs("FAILURE\n", stderr);

  delSolution(solution);
  delSolutionSet(solution_set);
  free(chiffres);

  return EXIT_SUCCESS;
}

