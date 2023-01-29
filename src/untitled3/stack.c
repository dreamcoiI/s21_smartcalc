/*!
\file
This file contains main functions needed for stack work
*/

#include "stack.h"

/**
 * Pushes new element to the end of stack.
 \param[in] stack Head element of the stack
 \param[in] is_num Tells if stack element is head element, parameter, number
 or x
 \param[in] num Double value. 0.0 if element is parameter
 \param[in] func Mathematical operator. NULL if element is number
 */

void push(Stack_t *stack, int is_num, double num, char *func) {
  Stack_t *tmp;
  tmp = (Stack_t *)calloc(1, sizeof(Stack_t));
  if (tmp != NULL) {
    Stack_t *tmp_stack = last_element_stack(stack);
    tmp->next = NULL;
    tmp->is_num = is_num;
    tmp->num = num;
    tmp->func = func;
    tmp_stack->next = tmp;
  } else {
    free(tmp);
  }
}

/**
 * Gets last (next element is NULL) element of the stack
 \param[in] stack Head element of the stack
 \return pointer to last element of the stack
 */

Stack_t *last_element_stack(Stack_t *tmp) {
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  return tmp;
}

/**
 * Gets length of the stack
 \param[in] stack Head element of the stack
 \return Length of the stack (head element is not included)
 */

int size_stack(Stack_t *stack) {
  int size = 0;
  while (stack->next != NULL) {
    stack = stack->next;
    size++;
  }
  return size;
}

/**
 * Frees stack with dynamically allocated operator values
 \param[in] stack Head element of the stack
 */

void free_stack(Stack_t *stack) {
  Stack_t *tmp1 = stack;
  Stack_t *tmp = stack;
  while (tmp->next != NULL) {
    tmp1 = tmp;
    tmp = tmp->next;
    free(tmp1->func);
    free(tmp1);
  }
}

/**
 * Frees stack with static operator values
 \param[in] stack Head element of the stack
 */

void fast_free_stack(Stack_t *stack) {
  Stack_t *tmp1 = stack;
  Stack_t *tmp = stack;
  while (tmp->next != NULL) {
    tmp1 = tmp;
    tmp = tmp->next;
    free(tmp1);
  }
}

/**
 * Unlinks last element of the stack without freeing oper content.
 * Does nothing stack is too short (consists only of head element).
 \param[in] stack Head element of the stack
 */

void unlock_last_element_stack(Stack_t *stack) {
  if (stack != NULL && stack->next != NULL) {
    while (stack->next->next != NULL) stack = stack->next;
    stack->next = NULL;
  }
}

/**
 * Unlinks last element of the first stack and links it to the end of second
 stack
 \param[in] unlock_last Head element of the first stack
 \param[in] stack Head element of the second stack
 */

void reunlock(Stack_t *unlock_last, Stack_t *stack) {
  Stack_t *unlast = last_element_stack(unlock_last);
  char *new_func;
  new_func = (char *)calloc(strlen(unlast->func) + 1, sizeof(char));
  strcpy(new_func, unlast->func);
  free(unlast);
  unlock_last_element_stack(unlock_last);
  push(stack, unlast->is_num, unlast->num, new_func);
}