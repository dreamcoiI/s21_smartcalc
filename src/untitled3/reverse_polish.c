/*!
\file
This file contains main functions needed for normalize value
*/

#include "stack.h"

/**
 * Makes reverse notation stack from given normal
 \param[in] stack Pointer to normal stack
 \return Pointer to head element of reversed stack.
 Does not free the original stack!
*/

Stack_t *to_polskiy(Stack_t *stack) {
  Stack_t *stack_next = stack->next;
  Stack_t *stack_prev = stack;
  Stack_t *stack_current;
  stack_current = (Stack_t *)calloc(1, sizeof(Stack_t));
  Stack_t *r_stack;
  r_stack = (Stack_t *)calloc(1, sizeof(Stack_t));
  stack_current->is_num = -1;
  r_stack->is_num = -1;
  while (stack_next != NULL) {
    // printf("here");
    if (stack_next->is_num != 0) push(r_stack, 1, stack_next->num, NULL);
    if (stack_next->is_num == 0) {
      int val_oper = value_operator(stack_next->func);
      if (val_oper == 0 || val_oper == 4) {
        push(stack_current, 0, 0, stack_next->func);
      } else if (val_oper == 1) {
        while (value_operator(last_element_stack(stack_current)->func) != 0)
          reunlock(stack_current, r_stack);
        free(last_element_stack(stack_current));
        unlock_last_element_stack(stack_current);
        if (size_stack(stack_current) > 0 &&
            value_operator(last_element_stack(stack_current)->func) == 4) {
          reunlock(stack_current, r_stack);
        }
      } else {
        while (size_stack(stack_current) > 0 &&
               (priority(stack_next->func) <
                    priority(last_element_stack(stack_current)->func) ||
                (priority(stack_next->func) ==
                     priority(last_element_stack(stack_current)->func) &&
                 left_assocate(stack_next->func)))) {
          reunlock(stack_current, r_stack);
        }
        push(stack_current, 0, 0, stack_next->func);
      }
    }
    stack_next = stack_next->next;
    stack_prev = stack_prev->next;
  }
  while (size_stack(stack_current) != 0) reunlock(stack_current, r_stack);
  fast_free_stack(stack_current);
  return r_stack;
}

/**
 * Returns operator priority
 \param[in] func Operation string
 \return Returns priority of operator.
 sin, cos, (, etc have lowest priority, ^ has highest
*/

int priority(char *func) {
  int func_code = -1;
  if (!strcmp(func, "+") || !strcmp(func, "-")) {
    func_code = 1;
  } else if (!strcmp(func, "*") || !strcmp(func, "/") || !strcmp(func, "mod")) {
    func_code = 2;
  } else if (!strcmp(func, "^")) {
    func_code = 3;
  }
  return func_code;
}

/**
 * Is operator left-associative?
 \param[in] func Operation string
 \return If operator is left-associative (-, /, mod, ^), 1 is returned.
 If not, 0
*/

int left_assocate(char *func) {
  return !strcmp(func, "-") || !strcmp(func, "/") || !strcmp(func, "mod") ||
         strcmp(func, "^");
}