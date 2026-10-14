#include <check.h>
#include <math.h>
#include <stdlib.h>

#include "../src/matrix.h"

// helpers

matrix_t create_test_matrix(int rows, int cols, double start_value) {
  matrix_t result;
  s21_create_matrix(rows, cols, &result);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      result.matrix[i][j] = start_value++;
    }
  }
  return result;
}

// ==================== CREATE MATRIX TESTS ====================
START_TEST(test_create_matrix_normal) {
  matrix_t m;
  int res = s21_create_matrix(3, 4, &m);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(m.rows, 3);
  ck_assert_int_eq(m.columns, 4);
  ck_assert_ptr_nonnull(m.matrix);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(m.matrix[i][j], 0.0);
    }
  }
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_1x1) {
  matrix_t m;
  int res = s21_create_matrix(1, 1, &m);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(m.rows, 1);
  ck_assert_int_eq(m.columns, 1);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_invalid_rows) {
  matrix_t m;
  int res = s21_create_matrix(0, 4, &m);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_create_matrix_invalid_cols) {
  matrix_t m;
  int res = s21_create_matrix(3, -1, &m);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_create_matrix_null_result) {
  int res = s21_create_matrix(3, 4, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

// ==================== EQ MATRIX TESTS ====================
START_TEST(test_eq_matrix_equal) {
  matrix_t A = create_test_matrix(3, 3, 1.0);
  matrix_t B = create_test_matrix(3, 3, 1.0);

  int res = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(res, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_not_equal_values) {
  matrix_t A = create_test_matrix(3, 3, 1.0);
  matrix_t B = create_test_matrix(3, 3, 2.0);

  int res = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_different_size) {
  matrix_t A = create_test_matrix(3, 3, 1.0);
  matrix_t B = create_test_matrix(3, 4, 1.0);

  int res = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_with_epsilon) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.1234567;
  B.matrix[0][0] = 1.1234568;
  A.matrix[0][1] = 2.0;
  B.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  B.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  B.matrix[1][1] = 4.0;

  int res = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(res, SUCCESS);

  B.matrix[0][0] = 1.12345;
  res = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_null_pointers) {
  matrix_t A = create_test_matrix(2, 2, 1.0);

  int res = s21_eq_matrix(&A, NULL);
  ck_assert_int_eq(res, FAILURE);

  res = s21_eq_matrix(NULL, &A);
  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
}
END_TEST

// ==================== SUM MATRIX TESTS ====================
START_TEST(test_sum_matrix_normal) {
  matrix_t A = create_test_matrix(2, 2, 1.0);
  matrix_t B = create_test_matrix(2, 2, 5.0);
  matrix_t result;

  int res = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 6.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 10.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 12.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_different_size) {
  matrix_t A = create_test_matrix(2, 2, 1.0);
  matrix_t B = create_test_matrix(3, 3, 1.0);
  matrix_t result;

  int res = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_null_pointers) {
  matrix_t A = create_test_matrix(2, 2, 1.0);
  matrix_t B = create_test_matrix(2, 2, 1.0);
  matrix_t result;

  int res = s21_sum_matrix(NULL, &B, &result);
  ck_assert_int_eq(res, 1);

  res = s21_sum_matrix(&A, NULL, &result);
  ck_assert_int_eq(res, 1);

  res = s21_sum_matrix(&A, &B, NULL);
  ck_assert_int_eq(res, 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// ==================== SUB MATRIX TESTS ====================
START_TEST(test_sub_matrix_normal) {
  matrix_t A = create_test_matrix(2, 2, 10.0);
  matrix_t B = create_test_matrix(2, 2, 1.0);
  matrix_t result;

  int res = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 9.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 9.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 9.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 9.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

// ==================== MULT NUMBER TESTS ====================
START_TEST(test_mult_number_normal) {
  matrix_t A = create_test_matrix(2, 2, 1.0);
  matrix_t result;

  int res = s21_mult_number(&A, 2.5, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 2.5, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 5.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 7.5, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 10.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_zero) {
  matrix_t A = create_test_matrix(2, 2, 1.0);
  matrix_t result;

  int res = s21_mult_number(&A, 0.0, &result);
  ck_assert_int_eq(res, 0);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], 0.0, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

// ==================== MULT MATRIX TESTS ====================
START_TEST(test_mult_matrix_normal) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;

  B.matrix[0][0] = 7;
  B.matrix[0][1] = 8;
  B.matrix[1][0] = 9;
  B.matrix[1][1] = 10;
  B.matrix[2][0] = 11;
  B.matrix[2][1] = 12;

  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 58.0, 1e-6);  // 1*7 + 2*9 + 3*11
  ck_assert_double_eq_tol(result.matrix[0][1], 64.0,
                          1e-6);  // 1*8 + 2*10 + 3*12
  ck_assert_double_eq_tol(result.matrix[1][0], 139.0,
                          1e-6);  // 4*7 + 5*9 + 6*11
  ck_assert_double_eq_tol(result.matrix[1][1], 154.0,
                          1e-6);  // 4*8 + 5*10 + 6*12

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
  matrix_t A = create_test_matrix(2, 3, 1.0);
  matrix_t B = create_test_matrix(2, 3, 1.0);
  matrix_t result;

  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// ==================== TRANSPOSE TESTS ====================
START_TEST(test_transpose_normal) {
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;

  int res = s21_transpose(&A, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);

  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 2.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 5.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][0], 3.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][1], 6.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_transpose_square) {
  matrix_t A = create_test_matrix(3, 3, 1.0);
  matrix_t result;

  int res = s21_transpose(&A, &result);
  ck_assert_int_eq(res, 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(result.matrix[j][i], A.matrix[i][j], 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

// ==================== DETERMINANT TESTS ====================
START_TEST(test_determinant_1x1) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  double det;
  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq_tol(det, 5.0, 1e-6);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2) {
  matrix_t A;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  double det;
  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq_tol(det, -2.0, 1e-6);  // 1*4 - 2*3 = -2

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;

  double det;
  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq_tol(det, 0.0, 1e-6);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_non_square) {
  matrix_t A = create_test_matrix(2, 3, 1.0);
  double det;

  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
}
END_TEST

// ==================== CALC COMPLEMENTS TESTS ====================
START_TEST(test_calc_complements_3x3) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;

  int res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 10.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][2], -20.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], -14.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][2], 8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][0], -8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][1], -2.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][2], 4.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_2x2) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  int res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], -3.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], -2.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 1.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  matrix_t A, result;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 1;

  int res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 1, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_non_square) {
  matrix_t A = create_test_matrix(2, 3, 1.0);
  matrix_t result;

  int res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
}
END_TEST

// ==================== INVERSE MATRIX TESTS ====================
START_TEST(test_inverse_matrix_3x3) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  int res = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], -1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][2], 1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], -38.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 41.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][2], -34.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][0], 27.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][1], -29.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][2], 24.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_matrix_singular) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t result;

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;

  int res = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_2x2) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  int res = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(res, 0);

  matrix_t identity, mult_result;
  s21_create_matrix(2, 2, &identity);

  s21_mult_matrix(&A, &result, &mult_result);

  ck_assert_double_eq_tol(mult_result.matrix[0][0], 1.0, 1e-6);
  ck_assert_double_eq_tol(mult_result.matrix[0][1], 0.0, 1e-6);
  ck_assert_double_eq_tol(mult_result.matrix[1][0], 0.0, 1e-6);
  ck_assert_double_eq_tol(mult_result.matrix[1][1], 1.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&identity);
  s21_remove_matrix(&mult_result);
}
END_TEST

// ==================== REMOVE MATRIX TESTS ====================
START_TEST(test_remove_matrix) {
  matrix_t m;
  s21_create_matrix(3, 3, &m);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      m.matrix[i][j] = i * 3 + j + 1;
    }
  }

  s21_remove_matrix(&m);

  ck_assert_ptr_null(m.matrix);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

// ==================== INTEGRATION TESTS ====================
START_TEST(test_integration_operations) {
  matrix_t A, B, sum_result, sub_result, mult_result, transpose_result;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  B.matrix[0][0] = 5;
  B.matrix[0][1] = 6;
  B.matrix[1][0] = 7;
  B.matrix[1][1] = 8;

  s21_sum_matrix(&A, &B, &sum_result);
  ck_assert_double_eq_tol(sum_result.matrix[0][0], 6.0, 1e-6);
  ck_assert_double_eq_tol(sum_result.matrix[0][1], 8.0, 1e-6);

  s21_sub_matrix(&A, &B, &sub_result);
  ck_assert_double_eq_tol(sub_result.matrix[0][0], -4.0, 1e-6);

  s21_mult_matrix(&A, &B, &mult_result);
  ck_assert_double_eq_tol(mult_result.matrix[0][0], 19.0, 1e-6);  // 1*5 + 2*7

  s21_transpose(&A, &transpose_result);
  ck_assert_double_eq_tol(transpose_result.matrix[0][1], 3.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&sum_result);
  s21_remove_matrix(&sub_result);
  s21_remove_matrix(&mult_result);
  s21_remove_matrix(&transpose_result);
}
END_TEST

// ==================== EDGE CASES TESTS ====================
START_TEST(test_large_matrix_operations) {
  matrix_t A, B, result;
  int size = 10;

  s21_create_matrix(size, size, &A);
  s21_create_matrix(size, size, &B);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      A.matrix[i][j] = i * size + j;
      B.matrix[i][j] = 1.0;
    }
  }

  int res = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 0);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], A.matrix[i][j] + 1.0, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_matrix_with_negative_values) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = -1;
  A.matrix[0][1] = -2;
  A.matrix[1][0] = -3;
  A.matrix[1][1] = -4;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 4;

  s21_sum_matrix(&A, &B, &result);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], 0.0, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

// ==================== TEST SUITE CREATION ====================
Suite* matrix_suite(void) {
  Suite* s = suite_create("Matrix Tests");

  // Create Matrix Tests
  TCase* tc_create = tcase_create("Create Matrix");
  tcase_add_test(tc_create, test_create_matrix_normal);
  tcase_add_test(tc_create, test_create_matrix_1x1);
  tcase_add_test(tc_create, test_create_matrix_invalid_rows);
  tcase_add_test(tc_create, test_create_matrix_invalid_cols);
  tcase_add_test(tc_create, test_create_matrix_null_result);
  suite_add_tcase(s, tc_create);

  // Eq Matrix Tests
  TCase* tc_eq = tcase_create("Eq Matrix");
  tcase_add_test(tc_eq, test_eq_matrix_equal);
  tcase_add_test(tc_eq, test_eq_matrix_not_equal_values);
  tcase_add_test(tc_eq, test_eq_matrix_different_size);
  tcase_add_test(tc_eq, test_eq_matrix_with_epsilon);
  tcase_add_test(tc_eq, test_eq_matrix_null_pointers);
  suite_add_tcase(s, tc_eq);

  // Sum Matrix Tests
  TCase* tc_sum = tcase_create("Sum Matrix");
  tcase_add_test(tc_sum, test_sum_matrix_normal);
  tcase_add_test(tc_sum, test_sum_matrix_different_size);
  tcase_add_test(tc_sum, test_sum_matrix_null_pointers);
  suite_add_tcase(s, tc_sum);

  // Sub Matrix Tests
  TCase* tc_sub = tcase_create("Sub Matrix");
  tcase_add_test(tc_sub, test_sub_matrix_normal);
  suite_add_tcase(s, tc_sub);

  // Mult Number Tests
  TCase* tc_mult_num = tcase_create("Mult Number");
  tcase_add_test(tc_mult_num, test_mult_number_normal);
  tcase_add_test(tc_mult_num, test_mult_number_zero);
  suite_add_tcase(s, tc_mult_num);

  // Mult Matrix Tests
  TCase* tc_mult = tcase_create("Mult Matrix");
  tcase_add_test(tc_mult, test_mult_matrix_normal);
  tcase_add_test(tc_mult, test_mult_matrix_incompatible);
  suite_add_tcase(s, tc_mult);

  // Transpose Tests
  TCase* tc_transpose = tcase_create("Transpose");
  tcase_add_test(tc_transpose, test_transpose_normal);
  tcase_add_test(tc_transpose, test_transpose_square);
  suite_add_tcase(s, tc_transpose);

  // Determinant Tests
  TCase* tc_det = tcase_create("Determinant");
  tcase_add_test(tc_det, test_determinant_1x1);
  tcase_add_test(tc_det, test_determinant_2x2);
  tcase_add_test(tc_det, test_determinant_3x3);
  tcase_add_test(tc_det, test_determinant_non_square);
  suite_add_tcase(s, tc_det);

  // Calc Complements Tests
  TCase* tc_calc = tcase_create("Calc Complements");
  tcase_add_test(tc_calc, test_calc_complements_3x3);
  tcase_add_test(tc_calc, test_calc_complements_2x2);
  tcase_add_test(tc_calc, test_calc_complements_1x1);
  tcase_add_test(tc_calc, test_calc_complements_non_square);
  suite_add_tcase(s, tc_calc);

  // Inverse Matrix Tests
  TCase* tc_inverse = tcase_create("Inverse Matrix");
  tcase_add_test(tc_inverse, test_inverse_matrix_3x3);
  tcase_add_test(tc_inverse, test_inverse_matrix_singular);
  tcase_add_test(tc_inverse, test_inverse_matrix_2x2);
  suite_add_tcase(s, tc_inverse);

  // Remove Matrix Tests
  TCase* tc_remove = tcase_create("Remove Matrix");
  tcase_add_test(tc_remove, test_remove_matrix);
  suite_add_tcase(s, tc_remove);

  // Integration Tests
  TCase* tc_integration = tcase_create("Integration");
  tcase_add_test(tc_integration, test_integration_operations);
  tcase_add_test(tc_integration, test_large_matrix_operations);
  tcase_add_test(tc_integration, test_matrix_with_negative_values);
  suite_add_tcase(s, tc_integration);

  return s;
}

int main(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = matrix_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
