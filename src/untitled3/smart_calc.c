/*!
\file
This file contains main functions needed for calculator work
*/

#include "stack.h"

/**
 * Calculates expression from string
 \param[in] str Expression string
 \param[out] res Actual answer
 \param[in] x_flag Expression x value
 \return Returns 0 if everything is calculated correctly
*/

int calcsmart(const char* str, double* res, double x_flag) {
  int err_code = 0;
  Stack_t* stack;
  stack = (Stack_t*)calloc(1, sizeof(Stack_t));
  if (stack != NULL) {
    stack->is_num = -1;
    err_code = str_pars(str, stack, x_flag);
    if (err_code == 0 && is_x_catch_there(stack) == 0) {
      err_code = smartcalc_with_stack(stack, res);
    } else {
      err_code = 1;
    }
  } else {
    err_code = 1;
  }
  free_stack(stack);
  return err_code;
}

/**
 * Calculates expression from stack
 \param[in] stack Expression stack (head element)
 \param[out] res Actual answer
 \return Returns 0 if everything is calculated correctly, 1 if something is
 wrong, 2 if there is zero division
*/

int smartcalc_with_stack(Stack_t* stack, double* res) {
  int err_code = 0;
  int flag = check_all(stack);
  if (flag == 0) {
    normal_for_calc(stack);
    Stack_t* st2 = to_polskiy(stack);
    flag = calc_from_notation(st2, res);
    if (*res != *res || isinf(*res)) err_code = 2;
    if (flag == 1) err_code = 3;
    free_stack(st2);
  } else {
    err_code = 1;
  }
  return err_code;
}

/**
 * Parses string into stack
 \param[in] str Input string
 \param[out] stack Head element of expression stack
 \param[in] x_flag Expression x value
 \return Returns 0 if there is no typos in original expressions
*/

int str_pars(const char* str, Stack_t* stack, double x) {
  int err_code = 0;
  while (str && *str != '\0' && err_code == 0) {
    size_t i = 0;
    if (('0' <= *str && *str <= '9') || *str == '.') {
      double num = 0.0;
      err_code = its_num_func(str, &num, &i);
      push(stack, 1, num, NULL);
    } else if (strchr("(+-*/^)acdgilmnoqrst", *str) != NULL) {
      char* func;
      err_code = its_func_func(str, &func, &i);
      if (strlen(func) > 1 && func[strlen(func - 1)] == '(') {
        func[strlen(func - 1)] = 0;
        push(stack, 0, 0.0, func);
        char* func2 = (char*)calloc(2, sizeof(char));
        if (func2 != NULL) {
          *func2 = '(';
          push(stack, 0, 0.0, func2);
        } else {
          err_code = 1;
          free(func2);
        }
      } else {
        push(stack, 0, 0.0, func);
      }
    } else if (*str == 'x' || *str == 'X') {
      push(stack, 1, x, NULL);
      i = 1;
    } else if (*str == ' ') {
      i = 1;
    } else {
      err_code = 1;
    }
    str += i;
  }
  return err_code;
}

/**
 * Extracts double value starting from indexed char
 \param[in] str Full nput string
 \param[in,out] len Position to start. Shifts to the end of number
 \param[out] num Extracted number
 \return Returns 0 if number is extracted, 1 if there is more than one dot
*/

int its_num_func(const char* str, double* num, size_t* len) {
  int err_code = 0;
  char* s;
  s = (char*)calloc(255, sizeof(char));
  if (s != NULL) {
    size_t i = 0;
    while (str && (('0' <= *str && *str <= '9') || *str == '.') &&
           err_code == 0) {
      if (*str == '.' && strchr(s, '.') != NULL) {
        err_code = 1;
      } else {
        s[i] = *str;
        i++;
        str++;
      }
    }
    *len = i;
    *num = atof(s);
    str++;
  } else {
    err_code = 1;
  }
  free(s);
  return err_code;
}

/**
 * Extracts mathematical operator starting from indexed char
 \param[in] str Full nput string
 \param[in,out] i Position to start. Shifts to the end of operator
 \param[out] src Extracted operator
 \return Returns 0 if operator is extracted,\n 1 if there is a typo,\n 2 if
 opening bracket is missing from sin, cos, etc.\n 3 if there is an allocation
 error
*/

int its_func_func(const char* str, char** src, size_t* i) {
  int err_code = 0;
  if (strchr("(+-*/^)", *str) != NULL) {
    (*src) = (char*)calloc(2, sizeof(char));
    (*src)[0] = *str;
    (*src)[1] = '\0';
    *i += 1;
  } else if (*str == 'm' && *(str + 1) == 'o' && *(str + 2) == 'd') {
    (*src) = (char*)calloc(4, sizeof(char));
    strcpy(*src, "mod");
    (*src)[3] = '\0';
    str += 2;
    *i += 3;
  } else {
    (*src) = (char*)calloc(5, sizeof(char));
    while (str && *str != '\0' && strchr("acdgilmnoqrst", *str) != NULL) {
      (*src)[*i] = *str;
      str++;
      *i += 1;
    }
    err_code = check_func(*src);
    if (*str == '(') {
      err_code = 0;
    } else if (err_code == 0) {
      err_code = 2;
    }
  }
  return err_code;
}

/**
 * Checks if function makes sense
 \param[in] src Function string
 \return Returns 0 if it is some known function, 1 if function does not make
 sense
*/

int check_func(char* src) {
  int err_code = 0;
  if (strcmp(src, "cos") && strcmp(src, "sin") && strcmp(src, "tan") &&
      strcmp(src, "acos") && strcmp(src, "asin") && strcmp(src, "atan") &&
      strcmp(src, "sqrt") && strcmp(src, "ln") && strcmp(src, "log")) {
    err_code = 1;
  }
  return err_code;
}

/**
 * Tells if parsed expression contains x
 \param[in] stack Head element of the stack
 \return 1 if expression contains x, 0 if not
*/

int is_x_catch_there(Stack_t* stack) {
  int err_code = 0;
  while (stack->next != NULL) {
    stack = stack->next;
    if (stack->is_num == 2) err_code = 1;
  }
  return err_code;
}
