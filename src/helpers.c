#include "matrix.h"

void s21_get_minor(matrix_t* A, matrix_t* minor, int row, int col) {
  s21_create_matrix(A->rows - 1, A->columns - 1, minor);

  for (int i = 0; i < A->rows - 1; i++) {
    for (int j = 0; j < A->columns - 1; j++) {
      int src_i = i < row ? i : i + 1;
      int src_j = j < col ? j : j + 1;
      minor->matrix[i][j] = A->matrix[src_i][src_j];
    }
  }
}

int s21_gauss_determinant(matrix_t* A, double* result) {
  matrix_t M;
  double det = 1.0;
  int errorCode = OK;

  s21_create_matrix(A->rows, A->columns, &M);

  for (int i = 0; i < A->rows; i++)
    for (int j = 0; j < A->columns; j++) M.matrix[i][j] = A->matrix[i][j];

  for (int i = 0; i < M.rows && det != 0.0; i++) {
    int p = i;

    for (int j = i + 1; j < M.rows; j++)
      if (fabs(M.matrix[j][i]) > fabs(M.matrix[p][i])) p = j;

    if (fabs(M.matrix[p][i]) < EPS)
      det = 0.0;
    else {
      if (p != i) {
        s21_swap_rows(&M, i, p);
        det = -det;
      }
      det *= M.matrix[i][i];
      s21_eliminate(&M, i);
    }
  }

  s21_remove_matrix(&M);
  *result = det;
  return errorCode;
}

void s21_swap_rows(matrix_t* A, int r1, int r2) {
  for (int j = 0; j < A->columns; j++) {
    double t = A->matrix[r1][j];
    A->matrix[r1][j] = A->matrix[r2][j];
    A->matrix[r2][j] = t;
  }
}

void s21_eliminate(matrix_t* A, int i) {
  for (int j = i + 1; j < A->rows; j++) {
    double f = A->matrix[j][i] / A->matrix[i][i];
    for (int k = i; k < A->columns; k++) A->matrix[j][k] -= f * A->matrix[i][k];
  }
}

int s21_is_matrix_valid(const matrix_t* A) {
  int errorCode = OK;
  if (A == NULL || A->matrix == NULL || A->rows < 1 || A->columns < 1) {
    errorCode = INCORRECT_MATRIX;
  } else {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i] == NULL) {
        errorCode = INCORRECT_MATRIX;
      }
    }
  }
  return errorCode;
}

int s21_is_matrix_square(const matrix_t* A) { return (A->rows == A->columns); }
