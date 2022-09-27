#include "handle_input_arguments.h"
#include <stdio.h>

#include "print_match.h"

void print_lines(char *line, int line_number, int offset, Flags *f_arguments, Line *lines_list, Line *current)
{
  if (f_arguments->is_c == 1) {
    // the argument c runs over all other printing formats (n,b),
    // so function returns without any more printing.
    return;
  }
  if (f_arguments->is_n == 1) {
    nHandler(f_arguments, line_number, current);
  }
  if (f_arguments->is_b == 1) {
    printf("%d:", offset);
  }
  printf("%s", line);
}

int send_line_to_print(Line *lines_list, Flags *f_arguments)
{
  Line *iter = lines_list;
  int i = 0;
  int count_printed_lines = 0;
  while (iter != NULL) {
    if (iter->is_match == 1) {
      print_lines(iter->data, iter->line_number, iter->offset, f_arguments, lines_list, iter);
      count_printed_lines++;
      if (f_arguments->A_num != 0) {
        count_printed_lines = handle_A_num(iter, f_arguments, count_printed_lines, lines_list);
      }
    }
    // no match - no print. go to next line.
    if (iter != NULL) {
      iter = iter->next;
    }
  }
  return count_printed_lines;
}
