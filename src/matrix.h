#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0

#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2

#define EPS 1e-6

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

// arithmetic functions
int s21_create_matrix(int rows, int columns, matrix_t* result);
void s21_remove_matrix(matrix_t* A);
int s21_eq_matrix(const matrix_t* A, const matrix_t* B);
int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_mult_number(matrix_t* A, double number, matrix_t* result);
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_transpose(matrix_t* A, matrix_t* result);
int s21_determinant(matrix_t* A, double* result);
int s21_calc_complements(matrix_t* A, matrix_t* result);
int s21_inverse_matrix(matrix_t* A, matrix_t* result);

// helper functions
int s21_is_matrix_valid(const matrix_t* A);
int s21_is_matrix_square(const matrix_t* A);
void s21_get_minor(matrix_t* A, matrix_t* minor, int row, int col);
int s21_gauss_determinant(matrix_t* A, double* result);
void s21_eliminate(matrix_t* m, int i);
void s21_swap_rows(matrix_t* m, int r1, int r2);

#endif