#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>

//#define MMIO_IMPLEMENTATION
//#include "mmio.h"

#define MAX_LINE_LENGTH 1024

int mm_read_mtx_crd_size(FILE *f, int *M, int *N, int *nz ) {
    char line[MAX_LINE_LENGTH];
    *M = *N = *nz = 0;
    do {
        if (fgets(line, MAX_LINE_LENGTH, f) == NULL) 
            return 1;
    } while (line[0] == '%');

    return sscanf(line, "%d %d %d", M, N, nz);
}

double compute_average_row_length(uint32_t* counts, int n) {
  double avg = 0.0;
#pragma omp parallel for reduction(+:avg)
  for (int i = 0; i < n; i++) {
    avg += (double)counts[i];
  }
  return avg / (double)n;
}

double compute_cov_row_length(uint32_t* counts, int n, double avg) {
  double avg_sq = 0.0;
#pragma omp parallel for reduction(+:avg_sq)
  for (int i = 0; i < n; i++) {
    avg_sq += (double)(counts[i] * counts[i]);
  }
  double variance = (avg_sq / (double)n) - (avg * avg);
  return sqrt(variance) / avg;
}

void get_stat_matrix(char* matrix_path) {
  FILE *f;
  uint32_t x, y;
  int M, N, nz, i;   
  double val, average_row_length, cov_row_length;

  if ((f = fopen(matrix_path, "r")) == NULL) {
    fprintf(stderr, "Could not read `%s`\n", matrix_path);
    exit(1);
  }

  if (mm_read_mtx_crd_size(f, &M, &N, &nz) != 3)
    exit(1);

  uint32_t* counts = (uint32_t*) calloc(M, sizeof(uint32_t)); 
  for (i = 0; i < nz; i++) {
    fscanf(f, "%d %d %lg\n", &x, &y, &val);
    counts[x - 1]++;
  }

  average_row_length = compute_average_row_length(counts, M);
  cov_row_length = compute_cov_row_length(counts, M, average_row_length);

  if (f !=stdin) fclose(f);
  free(counts);

  printf("%s, %d, %d, %d, %f, %f\n", matrix_path, M, N, nz, average_row_length, cov_row_length);
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [martix-market-filenames]\n", argv[0]);
    exit(1);
  }

  printf("matrix, M, N, nz, avg_rl, cov_rl\n");
#pragma omp parallel for
  for (int idx = 1; idx < argc; idx++) {
    get_stat_matrix(argv[idx]);
  }

  return 0;
}

