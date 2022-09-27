
#ifndef PRINT_MATCH_H
#define PRINT_MATCH_H
#include "store_input_data.h"

void print_lines(char *line, int line_number, int offset, Flags *f_arguments, Line *lines_list, Line *current);
int send_line_to_print(Line *lines_list, Flags *f_arguments);

#endif
