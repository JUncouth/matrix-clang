#include "matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t* result) {
  if (result == NULL || rows < 1 || columns < 1) {
    return INCORRECT_MATRIX;
  }

  int errorCode = OK;
  result->rows = rows;
  result->columns = columns;
  result->matrix = (double**)calloc(rows, sizeof(double*));

  if (result->matrix == NULL) {
    errorCode = INCORRECT_MATRIX;
  } else {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double*)calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        for (int j = 0; j < i; j++) free(result->matrix[j]);
        free(result->matrix);
        result->matrix = NULL;
        errorCode = INCORRECT_MATRIX;
      }
    }
  }

  return errorCode;
}

void s21_remove_matrix(matrix_t* A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(const matrix_t* A, const matrix_t* B) {
  if (s21_is_matrix_valid(A) != OK || s21_is_matrix_valid(B) != OK)
    return FAILURE;
  int errorCode = SUCCESS;
  if (A->rows != B->rows || A->columns != B->columns) errorCode = FAILURE;

  if (errorCode == SUCCESS) {
    for (int i = 0; errorCode == SUCCESS && i < A->rows; i++) {
      for (int j = 0; errorCode == SUCCESS && j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) >= EPS) errorCode = FAILURE;
      }
    }
  }

  return errorCode;
}

int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK || s21_is_matrix_valid(B) != OK)
    return INCORRECT_MATRIX;
  int errorCode = OK;
  if (A->rows != B->rows || A->columns != B->columns)
    errorCode = CALCULATION_ERROR;

  if ((errorCode == OK) &&
      ((errorCode = s21_create_matrix(A->rows, A->columns, result)) == OK)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return errorCode;
}

int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK || s21_is_matrix_valid(B) != OK)
    return INCORRECT_MATRIX;

  int errorCode = OK;
  if (A->rows != B->rows || A->columns != B->columns)
    errorCode = CALCULATION_ERROR;

  if ((errorCode == OK) &&
      ((errorCode = s21_create_matrix(A->rows, A->columns, result)) == OK)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return errorCode;
}

int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK) return INCORRECT_MATRIX;

  int errorCode;
  if ((errorCode = s21_create_matrix(A->rows, A->columns, result)) == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return errorCode;
}

int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK || s21_is_matrix_valid(B) != OK)
    return INCORRECT_MATRIX;

  int errorCode = OK;
  if (A->columns != B->rows) errorCode = CALCULATION_ERROR;

  if ((errorCode == OK) &&
      ((errorCode = s21_create_matrix(A->rows, B->columns, result)) == OK)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }

  return errorCode;
}

int s21_transpose(matrix_t* A, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK) return INCORRECT_MATRIX;

  int errorCode;
  if ((errorCode = s21_create_matrix(A->columns, A->rows, result)) == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return errorCode;
}

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK) return INCORRECT_MATRIX;

  int errorCode = OK;

  if (!s21_is_matrix_square(A)) {
    errorCode = CALCULATION_ERROR;
  } else if ((errorCode = s21_create_matrix(A->rows, A->columns, result)) ==
                 OK &&
             (A->rows == 1)) {
    result->matrix[0][0] = 1;
  } else if ((errorCode == OK) && (A->rows > 1)) {
    matrix_t minor;
    double det = 0;

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        s21_get_minor(A, &minor, i, j);
        s21_determinant(&minor, &det);
        result->matrix[i][j] = det * pow(-1, i + j);
        s21_remove_matrix(&minor);
      }
    }
  }
  return errorCode;
}

int s21_determinant(matrix_t* A, double* result) {
  if (s21_is_matrix_valid(A) != OK) return INCORRECT_MATRIX;

  int errorCode = OK;
  if (!s21_is_matrix_square(A)) {
    errorCode = CALCULATION_ERROR;
  } else if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else
    errorCode = s21_gauss_determinant(A, result);

  return errorCode;
}

int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  if (s21_is_matrix_valid(A) != OK) return INCORRECT_MATRIX;

  int errorCode = OK;
  double det = 0;
  matrix_t complements = {0};
  matrix_t transposed = {0};
  if (!s21_is_matrix_square(A)) {
    errorCode = CALCULATION_ERROR;
  } else if (((errorCode = s21_determinant(A, &det)) == OK) &&
             (fabs(det) >= EPS)) {
    errorCode = s21_calc_complements(A, &complements);
  } else if ((errorCode == OK) && (fabs(det) < EPS)) {
    errorCode = CALCULATION_ERROR;
  }
  if (errorCode == OK) {
    s21_transpose(&complements, &transposed);
    s21_mult_number(&transposed, 1.0 / det, result);
  }

  if (complements.rows > 0) s21_remove_matrix(&complements);
  if (transposed.rows > 0) s21_remove_matrix(&transposed);

  return errorCode;
}
