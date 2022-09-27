#ifndef STORE_INPUT_DATA_H
#define STORE_INPUT_DATA_H
#include <stdio.h>

typedef struct Line {
  char *data;
  int is_match;
  int line_number;
  int offset;
  struct Line *next;
} Line;

typedef struct Flags {
  int is_i;
  int is_n;
  int is_E;
  int is_A;
  int A_num;
  int is_v;
  int is_c;
  int is_b;
  int is_x;
} Flags;

void free_structs(Line *list, Flags *flags);
FILE *get_input_file(char *file);
Flags *fill_flags_struct(int number_of_input_args, char **input_args);
Line *read_lines(Flags *f_arguments, FILE *p_file, char *key_word);

#endif  // STORE_INPUT_DATA_H
