/*!
\file
This file contains main functions needed to calculate graph points array
*/


#include "stack.h"

/**
 * Calculates X and Y arrays for graph points
 \param[in] func Input string
 \param[out] num Empty pointer for future X array
 \param[in] value1 Min x value
 \param[in] value2 Max x value
 \param[in] i Number of points
 \return Operation code: 0 if everything is OK
*/

int smartcalc_graph(const char* func, double** num, double value1,
                    double value2, int i) {
  int err_code = 0;
  Stack_t* stack;
  *num = (double*)calloc(1, sizeof(double));
  stack = (Stack_t*)calloc(1, sizeof(Stack_t));
  int return_code = str_pars(func, stack, 0);
  if (return_code != 0) {
    err_code = 1;
  } else {
    if (!is_x_catch_there(stack)) {
      err_code = calcsmart(func, num[0], 0);
      for (int a = 1; a < i; a++) {
        (*num)[a] = (*num)[0];
      }
    } else {
      free_stack(stack);
      double move = (value2 - value1) / (i - 1);
      int condition[i];
      for (int a = 0; a < i; a++) {
        stack = (Stack_t*)calloc(1, sizeof(Stack_t));
        stack->is_num = -1;
        str_pars(func, stack, 0);
        replace_x_all(stack, value1);
        double x = 0.0;
        condition[a] = smartcalc_with_stack(stack, &x);
        if (condition[a] == 0) {
          (*num)[a] = x;
        }
        value1 += move;
        free_stack(stack);
      }
      int count = 1;
      for (int a = 0; a < i; a++) {
        if (condition[a] == 0) {
          count = 0;
        }
      }
      err_code = count;
    }
  }
  return err_code;
}

/**
 * Replaces x for some number to calculate the expression
 \param[in] stack Head of stack
 \param[in] num Number to replace x
*/

void replace_x_all(Stack_t* stack, double num) {
  while (stack->next != NULL) {
    stack = stack->next;
    if (stack->is_num == 2) stack->num = num;
  }
}
