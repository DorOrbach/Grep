#include "print_match.h"
#include "store_input_data.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ARG_LOC_IN_ARRAY 1

int main(int argc, char *argv[])
{
  Line *lines_list = NULL;
  int number_of_args = argc;
  FILE *p_file = NULL;
  char *line = NULL;
  // create a struct holding all the arguments given:
  Flags *args_flags = fill_flags_struct(number_of_args, argv);
  // get the path to input text file:
  p_file = get_input_file(argv[argc - 1]);

  // handle the two cases where the key line is given last or before the file path:
  if (p_file == stdin) {
    lines_list = read_lines(args_flags, p_file, argv[argc - 1]);
  } else {
    lines_list = read_lines(args_flags, p_file, argv[argc - 2]);
  }

  int count_printed_lines = send_line_to_print(lines_list, args_flags);
  if (args_flags->is_c == 1) {
    printf("%d\n", count_printed_lines);
  }
  free_structs(lines_list, args_flags);
  exit(0);
}
