/*!
\file
This file contains functions needed to calculate value from reverse notation
stack
*/

#include "stack.h"
/**
 * Calculates double value from reverse notation stack
 \param[in] stack Head stack element
 \param[out] res Calculated value
 \return Operation code: 0 if everything is OK, 1 if stack broke
*/

int calc_from_notation(Stack_t* stack, double* res) {
  int err_code = 0;
  if (size_stack(stack) > 1) {
    while (stack != NULL && stack->next != NULL && stack->next->next != NULL &&
           size_stack(stack) > 1) {
      Stack_t* next = stack->next->next;
      Stack_t* current = stack->next;
      Stack_t* prev = stack;
      while (next != NULL && next->is_num != 0) {
        next = next->next;
        current = current->next;
        prev = prev->next;
      }
      if (next != NULL) {
        double res_func = calc_function(prev->num, current->num, next->func);
        if (!check_func(next->func)) {
          free(next->func);
          current->next = next->next;
          free(next);
          current->num = res_func;
        } else {
          free(next->func);
          prev->next = next->next;
          free(next);
          free(current);
          prev->num = res_func;
        }
      }
    }
  }
  if (stack != NULL && stack->next != NULL) {
    *(res) = stack->next->num;
  } else if (stack != NULL && stack->next == NULL) {
    *res = stack->num;
  } else {
    err_code = 1;
  }
  return err_code;
}

/**
 * Utility function. Calculates one step
 \param[in] num1 First number. Zero if operation takes only one number
 \param[in] num2 Second number
 \param[in] func Parameter in char * form
 \return Calculated value
*/

double calc_function(double num1, double num2, char* func) {
  double res = 0.0;
  if (!strcmp(func, "+")) res = num1 + num2;
  if (!strcmp(func, "-")) res = num1 - num2;
  if (!strcmp(func, "*")) res = num1 * num2;
  if (!strcmp(func, "/")) res = num1 / num2;
  if (!strcmp(func, "mod")) res = fmod(num1, num2);
  if (!strcmp(func, "^")) res = pow(num1, num2);
  if (!strcmp(func, "cos")) res = cos(num2);
  if (!strcmp(func, "sin")) res = sin(num2);
  if (!strcmp(func, "tan")) res = tan(num2);
  if (!strcmp(func, "acos")) res = acos(num2);
  if (!strcmp(func, "asin")) res = asin(num2);
  if (!strcmp(func, "atan")) res = atan(num2);
  if (!strcmp(func, "sqrt")) res = sqrt(num2);
  if (!strcmp(func, "ln")) res = log(num2);
  if (!strcmp(func, "log")) res = log10(num2);
  return res;
}
