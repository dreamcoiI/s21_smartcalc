/*! \mainpage SmartCalc_v1.0 by Helene Hotblack s21
 *
 * \section Project Description
 * Implementation of an extended version of the usual
 * calculator, which can be found in the standard applications of each operating
 * system in the C programming language using structured programming
 *
 * \section Commands
 * Run \a "make" or \a "make all" to build calculator\n
 * Run \a "make install" to install application on your Desktop\n
 * Run \a "make dvi" to get this documentation\n
 * Run \a "make test" to run tests\n
 * Run \a "make gcov_report" to get coverage report
 *
 */

/*!
\file
This file contains definitions of all C functions used in project
*/

#ifndef STACK_H
#define STACK_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Stack structure, used for storing the expression, stack and output string
 */
typedef struct Stack_t {
  //! Tells the type of stored value:\n
  //! -1 - head element, stores nothing, prevents memory problems;\n
  //! 0 - element stores mathematical operator;\n
  //! 1 - element stores number (double);\n
  //! 2 - elememt stores x (needed for graph dots calculation, in many cases
  //! interpreted as number)
  int is_num;
  //! Numeric value. Equals zero in operator elements
  double num;
  //! Operator value. Equals NULL if number is stored
  char *func;
  //! Next element of the stack. Equals NULL for the last element
  struct Stack_t *next;
} Stack_t;

void push(Stack_t *stack_head, int is_num, double num, char *func);
Stack_t *last_element_stack(Stack_t *tmp);
int size_stack(Stack_t *stack);
void free_stack(Stack_t *stack);
void fast_free_stack(Stack_t *stack);
void unlock_last_element_stack(Stack_t *stack);
void reunlock(Stack_t *unlock_last, Stack_t *stack);
void normal_for_calc(Stack_t *stack);
int calcsmart(const char *str, double *res, double x_flag);
int str_pars(const char *str, Stack_t *stack, double x);
int its_num_func(const char *str, double *num, size_t *len);
int smartcalc_with_stack(Stack_t *stack, double *res);
int is_x_catch_there(Stack_t *stack);
int check_func(char *src);
int check_normal_for_calc1(Stack_t *next_stack, Stack_t *prev_stack);
int check_normal_for_calc2(Stack_t *next_stack, Stack_t *prev_stack);
int value_operator(char *func);
// void is_negative(Stack_t *stack);
int func_check(Stack_t *stack);
int bracket_check(Stack_t *stack);
int check_all(Stack_t *stack);
int left_assocate(char *func);
int priority(char *func);
Stack_t *to_polskiy(Stack_t *stack);
double calc_function(double num1, double num2, char *func);
int smartcalc_graph(const char *func, double **num, double value1,
                    double value2, int i);
int calc_from_notation(Stack_t *stack, double *res);
void replace_x_all(Stack_t *stack, double num);
int its_func_func(const char *str, char **src, size_t *i);
#ifdef __cplusplus
}
#endif

#endif
