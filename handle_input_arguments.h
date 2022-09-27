
#ifndef HANDLE_INPUT_ARGUMENTS_H
#define HANDLE_INPUT_ARGUMENTS_H
#include "store_input_data.h"

int reverse_match_when_v(int match);
void nHandler(Flags *f_arguments, int lines_counter, Line *current);
int x_handler(char *line, char *key);
int handle_A_num(Line *current, Flags *f_arguments, int count_printed_lines, Line *lines_list);
char *upperToLower(char *s);

#endif
