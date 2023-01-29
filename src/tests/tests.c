#include "tests.h"

START_TEST(simple_sum) {
  double x;
  int code = calcsmart("2+2* 2", &x, 0);
  ck_assert_int_eq(code, 0);

  ck_assert_ldouble_eq_tol(x, 6.0, 1e-8);
}

START_TEST(brackets) {
  double x;
  int code = calcsmart("(2 + 2) * 2 - 2 + 2", &x, 0);
  ck_assert_int_eq(code, 0);
  ck_assert_ldouble_eq_tol(x, 8.0, 1e-8);
}

START_TEST(complex_operators) {
  double x;
  int code = calcsmart("sin(0) + 3 mod 2", &x, 0);
  ck_assert_int_eq(code, 0);
  ck_assert_ldouble_eq_tol(x, 1.0, 1e-8);
}

START_TEST(prior) {
  double x;
  int code = calcsmart("-1*sqrt(4)^2", &x, 0);
  ck_assert_int_eq(code, 0);
  ck_assert_ldouble_eq_tol(x, -4.0, 1e-8);
}

Suite* simple_and_right(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("correct expressions");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, simple_sum);
  tcase_add_test(tc_core, brackets);
  tcase_add_test(tc_core, complex_operators);
  tcase_add_test(tc_core, prior);

  suite_add_tcase(s, tc_core);
  return s;
}

START_TEST(too_many_brackets) {
  double x;
  int code = calcsmart("(2) + 2) * 2", &x, 0);
  ck_assert_int_ne(code, 0);
}

START_TEST(unparseable_string) {
  double x;
  int code = calcsmart("(2b + 2) * 2", &x, 0);
  ck_assert_int_ne(code, 0);
}
START_TEST(minus) {
  double x;
  int code = calcsmart("2+(-1)", &x, 0);
  ck_assert_int_eq(code, 0);
}

// START_TEST(x_came_too_early) {
//   double x;
//   int code = calcsmart("2*x + 4", &x, 0);
//   ck_assert_int_ne(code, 0);
// }

START_TEST(you_forgot_a_bracket) {
  double x;
  int code = calcsmart("sin0 + cos(4)", &x, 0);
  ck_assert_int_ne(code, 0);
}

Suite* wrong_expressions(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("wrong expressions");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, too_many_brackets);
  tcase_add_test(tc_core, unparseable_string);
  tcase_add_test(tc_core, minus);
  tcase_add_test(tc_core, you_forgot_a_bracket);

  suite_add_tcase(s, tc_core);
  return s;
}

// START_TEST(simple_graph) {
//   double* y;
//   int code = smartcalc_graph("sin(x) + cos(x)", &y, 0, 10.0, 11);
//   ck_assert_int_eq(code, 0);
//   double* true_x;
//   double* true_y;
//   true_x = (double*)calloc(10, sizeof(double));
//   true_y = (double*)calloc(10, sizeof(double));
//   true_y[0] = sin(0.0) + cos(0.0);
//   true_x[0] = 0.0;
//   ck_assert_ldouble_eq_tol(true_y[0], y[0], 1e-8);
//   for (int i = 1; i < 10; i++) {
//     true_x[i] = 1.0 + true_x[i - 1];
//     true_y[i] = sin(true_x[i]) + cos(true_x[i]);
//     ck_assert_ldouble_eq_tol(true_y[i], y[i], 1e-8);
//   }
//   free(y);
//   free(true_x);
//   free(true_y);
// }

// Suite* graph_suite(void) {
//   Suite* s;
//   TCase* tc_core;

//   s = suite_create("graph");

//   tc_core = tcase_create("Core");
//   tcase_add_test(tc_core, simple_graph);
//   suite_add_tcase(s, tc_core);
//   return s;
// }

int main() {
  int no_failed = 0;
  Suite *simple, *wrong;
  SRunner* runner;

  simple = simple_and_right();
  wrong = wrong_expressions();
  //   graph = graph_suite();

  runner = srunner_create(simple);
  srunner_add_suite(runner, wrong);
  //   srunner_add_suite(runner, graph);
  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);

  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}