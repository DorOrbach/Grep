#include "handle_input_arguments.h"
#include "print_match.h"
#include "store_input_data.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *upperToLower(char *s)
{
  char *p = s;
  for (p = s; *p; p++) {
    *p = tolower(*p);
  }
  return s;
}

int reverse_match_when_v(int match)
{
  if (match == 1) {
    return 0;
  }
  return 1;
}

void nHandler(Flags *f_arguments, int lines_counter, Line *current)
{
  if (f_arguments->is_n == 0) {
    return;
  }
  if (f_arguments->is_A == 1) {
    if (current->is_match == 0) {
      printf("%d-", lines_counter);
    } else {
      printf("%d:", lines_counter);
    }
  } else {
    printf("%d:", lines_counter);
  }
}

int x_handler(char *line, char *key)
{
  // before comparing line and key. replace '\n' with '\0' at the end of line:
  int i = 0;
  while (line[i] != '\n') {
    i++;
  }
  line[i] = '\0';
  // check with strcmp if line is exactly key.
  if (strcmp(line, key) == 0) {
    line[i] = '\n';
    return 1;
  }
  line[i] = '\n';
  return 0;
}

int check_if_more_match_lines(Line *lines_list)
{
  Line *iter = lines_list;
  if (iter == NULL) {
    return 0;
  }
  if (iter->next == NULL) {
    return 0;
  }
  iter = iter->next;
  while (iter != NULL) {
    if (iter->is_match == 1) {
      return 1;
    }
    iter = iter->next;
  }
  return 0;
}

int handle_A_num(Line *current, Flags *f_arguments, int count_printed_lines, Line *lines_list)
{
  int i = 0;
  if (current->next == NULL) {
    return count_printed_lines;
  }
  while (i < f_arguments->A_num) {
    current = current->next;

    // print current and return without advancing the list.
    if (current->next != NULL && current->next->is_match == 1) {
      print_lines(current->data, current->line_number, current->offset, f_arguments, lines_list, current);
      count_printed_lines++;
      return count_printed_lines;
    }

    // if we got to end of list or the next line should be printed, then no action. and return.
    if (current == NULL || current->is_match == 1) {
      return count_printed_lines;
    }
    print_lines(current->data, current->line_number, current->offset, f_arguments, lines_list, current);
    count_printed_lines++;
    i++;
  }
  if (current->next != NULL) {
    if (current->next->is_match == 0) {
      if (check_if_more_match_lines(current) == 1) {
        printf("--\n");
      }
    }
  }

  return count_printed_lines;
}
