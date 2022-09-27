#include "store_input_data.h"
#include "handle_input_arguments.h"
#include "regular_case_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_LOC_IN_ARRAY 1

Line *create_lines_struct(char *line, int lines_counter, int bytes_counter, int match)
{
  Line *current = NULL;
  current = (Line *)malloc(sizeof(Line) + strlen(line) + 1);
  if (current == NULL) {
    exit(1);
  }
  current->data = (char *)malloc(sizeof(char) * (strlen(line) + 1));
  if (current->data == NULL) {
    exit(1);
  }

  strcpy(current->data, line);
  current->is_match = match;
  current->line_number = lines_counter;
  current->offset = bytes_counter;
  current->next = NULL;
  return current;
}

Line *add_line_to_list(Line *lines_list, char *line, int lines_counter, int bytes_counter, int match)
{
  Line *tail = NULL, *new_line = create_lines_struct(line, lines_counter, bytes_counter, match);
  if (lines_list == NULL) {
    return new_line;
  }
  tail = lines_list;
  while (tail->next != NULL) {
    tail = tail->next;
  }
  tail->next = new_line;
  return lines_list;
}

Flags *create_flags_struct()
{
  Flags *flag = (Flags *)malloc(sizeof(Flags));
  if (flag == NULL) {
    exit(1);
  }
  flag->is_i = 0;
  flag->is_n = 0;
  flag->is_E = 0;
  flag->is_A = 0;
  flag->A_num = 0;
  flag->is_v = 0;
  flag->is_c = 0;
  flag->is_b = 0;
  flag->is_x = 0;
  return flag;
}
void free_structs(Line *list, Flags *flags)
{
  Line *temp = NULL;
  while (list != NULL) {
    temp = list;
    list = list->next;
    free(temp->data);
    free(temp);
  }

  free(flags);
}

// check of a current case is given to the grep function
int is_case_in_arguments(char *arguments[], int number_of_arguments, const char *case_to_check)
{
  int i = 1;

  while (i < number_of_arguments - 2) {
    if (case_to_check[ARG_LOC_IN_ARRAY] == arguments[i][ARG_LOC_IN_ARRAY]) {
      return 1;
    }
    i++;
  }
  return 0;
}

int get_A_num(char *arg[], int num_of_args)
{
  int A_num = 0;
  int i = 1;
  while (i < num_of_args - 1) {
    A_num = atoi(arg[i]);
    i++;
    if (A_num != 0) {
      return A_num;
    }
  }
  return A_num;
}

// fill the flags struct and create a struct which holds every input argument:
Flags *fill_flags_struct(int number_of_input_args, char **input_args)
{
  Flags *arguments_flags = create_flags_struct();
  // check for case i:
  if (is_case_in_arguments(input_args, number_of_input_args, "-i")) {
    arguments_flags->is_i = 1;
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-n")) {
    arguments_flags->is_n = 1;
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-E")) {
    arguments_flags->is_E = 1;
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-A")) {
    arguments_flags->is_A = 1;
    arguments_flags->A_num = get_A_num(input_args, number_of_input_args);
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-v")) {
    arguments_flags->is_v = 1;
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-c")) {
    arguments_flags->is_c = 1;
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-b")) {
    arguments_flags->is_b = 1;
  }
  if (is_case_in_arguments(input_args, number_of_input_args, "-x")) {
    arguments_flags->is_x = 1;
  }

  return arguments_flags;
}

// get the text file from argv or from stdin:
FILE *get_input_file(char *file)
{

  FILE *p_file = NULL;
  if (strcmp(file, "my_grep") == 0) {
    p_file = stdin;
    return p_file;
  }
  p_file = fopen(file, "r");
  if (NULL == p_file) {
    p_file = stdin;
  }
  if (NULL == p_file) {
    exit(1);
  }
  return p_file;
}

Line *read_lines(Flags *f_arguments, FILE *p_file, char *key_word)
{
  Line *lines_list = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int match = 0;
  int lines_counter = 0;
  int bytes_counter = 0;
  // save the key_word as a list of structs"
  int key_len = strlen(key_word);
  if (f_arguments->is_i == 1) {
    handle_i_arg(f_arguments, key_word);
  }
  Regular **keys = (Regular **)malloc(sizeof(Regular) + key_len * sizeof(Type) * sizeof(int));
  if (keys == NULL) {
    exit(1);
  }
  create_key_struct(key_word, keys);
  // start reading lines and store the data in as a struct:
  while ((read = getline(&line, &len, p_file)) != -1) {
    lines_counter++;
    match = check_match(keys, line, f_arguments);
    if (f_arguments->is_x == 1) {
      match = x_handler(line, key_word);
    }
    if (f_arguments->is_v == 1) {
      match = reverse_match_when_v(match);
    }
    // add the current line to a struct of line - works as linked list.
    lines_list = add_line_to_list(lines_list, line, lines_counter, bytes_counter, match);
    bytes_counter = bytes_counter + read;
  }
  fclose(p_file);
  free_all_memory(keys);
  free(keys);
  free(line);
  return lines_list;
}
