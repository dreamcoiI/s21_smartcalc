/*!
\file
This file contains main functions needed for calculate work
*/

#include "stack.h"

/**
 * Remove all unary + and reconstruct all unary -
 \param[in] stack Head stack element
*/

void normal_for_calc(Stack_t *stack) {
  Stack_t *next_stack = stack->next;
  Stack_t *prev_stack = stack;
  if (next_stack != NULL) {
    while (next_stack->next != NULL) {
      if (check_normal_for_calc1(next_stack, prev_stack)) {
        if (!strcmp(next_stack->func, "-"))
          next_stack->next->num *= -1;  
        Stack_t *next_stack2 = next_stack;
        next_stack = next_stack->next;
        prev_stack->next = next_stack;
        free(next_stack2->func);
        free(next_stack2);
      } else if (check_normal_for_calc2(next_stack, prev_stack)) {
        // is_negative(next_stack);
        next_stack = next_stack->next;
        prev_stack = prev_stack->next;
      } else {
        next_stack = next_stack->next;
        prev_stack = prev_stack->next;
      }
    }
  }
}

/**
 * It's just check for normal -
 \param[in] next_stack Head stack element
 \param[in] prev_stack Previous stack element
*/

int check_normal_for_calc1(Stack_t *next_stack, Stack_t *prev_stack) {
  int yes = 0;
  if (((prev_stack->is_num == 0 && prev_stack->func != NULL &&
        strcmp(prev_stack->func, ")")) ||
       prev_stack->is_num == -1) &&
      next_stack->is_num == 0 &&
      (!strcmp(next_stack->func, "+") ||
       (!strcmp(next_stack->func, "-") &&
        (next_stack->next->is_num == 1 || next_stack->next->is_num == 2)))) {
    yes = 1;
  }
  return yes;
}

/**
 * It's too just check for normal -
 \param[in] next_stack Head stack element
 \param[in] prev_stack Previous stack element
*/

int check_normal_for_calc2(Stack_t *next_stack, Stack_t *prev_stack) {
  int yes = 0;
  if ((prev_stack->is_num == 0 || prev_stack->is_num == -1) &&
      next_stack->is_num == 0 && next_stack->next->is_num == 0 &&
      ((value_operator(next_stack->next->func) == 0 ||
        value_operator(next_stack->next->func) == 4) &&
       (!strcmp(next_stack->func, "-"))))
    yes = 1;
  return yes;
}

/**
 * States the simplified type of the operator
 \param[in] func operator string
 \return Operator type code: 0 -- (; 1 -- ); 2 -- +, -; 3 -- *, /, ^, mod; 4 --
 other
 */

int value_operator(char *func) {
  int err_code = 4;
  if (!strcmp(func, "(")) {
    err_code = 0;
  } else if (!strcmp(func, ")")) {
    err_code = 1;
  } else if (!strcmp("+", func) || !strcmp("-", func)) {
    err_code = 2;
  } else if (!strcmp("^", func) || !strcmp("*", func) || !strcmp("/", func) ||
             !strcmp("mod", func)) {
    err_code = 3;
  }
  return err_code;
}

/**
 * Checks if expression can be calculated\n
 * Uses special code prev for defining the type of previous stack element:\n
 0 -- start of expression\n
 1 -- number\n
 2 -- opening bracket\n
 3 -- closing bracket\n
 4 -- simple operator (+, -, *, /, ^, mod)\n
 5 -- long operator (sin, cos and so on)\n
 \param[in] stack Head element of the stack
 \return Operation code. If code is 0, expression is correct and can be
 calculated. If not, something is wrong:\n
 1 -- there are two numbers without any operator in between or number goes right
 after closing brackets\n
 2 -- opening bracket goes right after number\n
 3 -- errors in ")" placement\n
 4 -- too many simple operators in a row\n
 5 -- errors in simple operator placement\n
 6 -- errors in function placement\n
 7 -- last element is not an closing bracket or number
 */

int func_check(Stack_t *stack) {
  int func_code = 0;
  int prev = 0;
  int count = 0;
  Stack_t *next_stack = stack->next;
  while (next_stack != NULL) {
    if (next_stack->is_num == 1 || next_stack->is_num == 2) {
      prev = 1;
      count = 0;
    } else {
      int small_counter = value_operator(next_stack->func);
      switch (small_counter) {
        case 0:
          if (prev == 1 || prev == 3) func_code = 2;
          prev = 2;
          count = 0;
          break;
        case 1:
          if (prev == 0 || prev == 2 || prev == 4 || prev == 5) func_code = 3;
          prev = 3;
          count = 0;
          break;
        case 2:
          if (count >= 2) func_code = 4;
          prev = 4;
          count++;
          break;
        case 3:
          if (prev == 0 || prev == 2 || prev == 4 || prev == 5) func_code = 5;
          prev = 4;
          count++;
          break;
        case 4:
          if (prev == 1 || prev == 3) func_code = 6;
          prev = 4;
          count = 0;
      }
    }
    next_stack = next_stack->next;
  }
  if (last_element_stack(stack)->is_num == 0 &&
      strcmp(last_element_stack(stack)->func, ")")) {
    func_code = 7;
  }
  return func_code;
}

/**
 * Checks if every bracket is closed correctly
 \param[in] stack Head element of the stack
 \return Operation code. If code is 0, all opened brackets are closed.
 If not, something is wrong: \n 1 -- not every bracket is closed\n
 2 -- there is "()" situation\n
 3 -- too many closing brackets somewhere in the expression
 */

int bracket_check(Stack_t *stack) {
  int func_code = 0;
  int bracket_count = 0;
  int r_bracket = 0;
  Stack_t *current_stack = stack;
  while (current_stack != NULL) {
    if (current_stack->is_num == 0 && current_stack->func != NULL &&
        !strcmp(current_stack->func, "(")) {
      bracket_count++;
      r_bracket = 1;
    } else if (current_stack->is_num == 0 && current_stack->func != NULL &&
               !strcmp(current_stack->func, ")")) {
      if (r_bracket == 1) func_code = 2;
      bracket_count--;
    } else {
      r_bracket = 0;
    }
    if (bracket_count < 0) func_code = 3;
    current_stack = current_stack->next;
  }
  if (bracket_count != 0) func_code = 1;
  return func_code;
}

/**
 * Checks if input expression can be calculated
 \param[in] stack Head element of the stack
 \return Operation code. If code is 0, expression is correct and can be
 calculated. If not, something is wrong with syntaxis
 */
int check_all(Stack_t *stack) {
  int a = 0, b = 0;
  a = bracket_check(stack);
  b = func_check(stack);
  printf("%d %d\n", a, b);
  return a + b;
}
