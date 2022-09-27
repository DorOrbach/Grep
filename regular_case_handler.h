
#ifndef REGULAR_CASE_HANDLER_H
#define REGULAR_CASE_HANDLER_H

#include "store_input_data.h"
#define SIMPLE_CHAR_LEN 3

typedef struct Circle {
  char* key1;
  char* key2;
} Circle;

typedef struct Squere {
  int start;
  int end;
} Squere;

typedef struct Simple_char {
  char simple_str[SIMPLE_CHAR_LEN];
} Simple_char;

typedef union Type {
  Circle* circle;
  Squere* squere;
  Simple_char* simple;
} Type;

typedef struct Regular {
  int is_init;
  int is_simple;
  int is_circle;
  int is_squere;
  int is_dot;
  Type type;
} Regular;

int check_match(Regular** keys, char* line_original, Flags* f_arguments);
Regular** create_key_struct(const char* key_word, Regular** keys);
void handle_i_arg(Flags* f_arguments, char* key);
void free_all_memory(Regular** keys);
#endif
