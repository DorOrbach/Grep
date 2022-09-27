#define _CRT_SECURE_NO_WARNINGS
#include "regular_case_handler.h"
#include "handle_input_arguments.h"
#include "store_input_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONE_INIT_FLAG 9
#define LAST_STRUCT 2
#define BACK_SLASH 92

Regular *create_struct()
{
  Regular *reg = (Regular *)malloc(sizeof(Regular));
  if (reg == NULL) {
    exit(1);
  }
  reg->is_init = 0;
  reg->is_circle = 0;
  reg->is_dot = 0;
  reg->is_simple = 0;
  reg->is_squere = 0;
  return reg;
}

char *extract_key_from_circle(char *temp, int sign)
{
  if (sign == 1) {
    char *key1;
    temp = strstr(temp, "(");
    temp++;
    if (temp[0] == ')') {
      key1 = " ";
      return key1;
    }
    key1 = strtok(temp, "|");
    int len = strlen(key1);
    key1[len] = '\0';
    return key1;
  }
  if (sign == 2) {
    char *key2, *p;
    p = strstr(temp, "|");
    p++;
    if (p[0] == ')') {
      key2 = " ";
      return key2;
    }
    key2 = strtok(p, ")");
    int len = strlen(key2);
    key2[len] = '\0';
    return key2;
  }
}

Regular **add_simple_char_to_keys(Regular **arr, int keys_struct_index, const char *key, int key_index)
{
  int j = keys_struct_index;
  arr[j] = create_struct();
  arr[j]->is_simple = 1;
  arr[j]->is_init = 1;
  arr[j]->type.simple = (Simple_char *)malloc(sizeof(Simple_char));
  if (arr[j]->type.simple == NULL) {
    exit(1);
  }
  arr[j]->type.simple->simple_str[0] = key[key_index];
  arr[j]->type.simple->simple_str[1] = '\0';
  return arr;
}
Regular **add_squere_range_to_keys(Regular **arr, int keys_struct_index, char *key, int key_index)
{
  int j = keys_struct_index;
  arr[j] = create_struct();
  arr[j]->is_squere = 1;
  arr[j]->is_init = 1;
  arr[j]->type.squere = (Squere *)malloc(sizeof(Squere));
  if (arr[j]->type.squere == NULL) {
    exit(1);
  }
  return arr;
}
Regular **add_circle_keys_to_struct(Regular **arr, int keys_struct_index, char *key, int key_index)
{
  int len = strlen(key);
  int j = keys_struct_index;
  char *temp1 = (char *)malloc(sizeof(char) * len + 1);
  char *key1 = (char *)malloc(sizeof(char) * len + 1);
  if (temp1 == NULL || key1 == NULL) {
    exit(1);
  }
  strcpy(temp1, key);
  strcpy(key1, extract_key_from_circle(temp1, 1));

  char *temp2 = (char *)malloc(sizeof(char) * len + 1);
  char *key2 = (char *)malloc(sizeof(char) * len + 1);
  if (temp2 == NULL || key2 == NULL) {
    exit(1);
  }
  strcpy(temp2, key);
  strcpy(key2, extract_key_from_circle(temp2, 2));
  free(temp1);
  free(temp2);

  arr[j] = create_struct();
  arr[j]->type.circle = (Circle *)malloc(sizeof(Circle));
  if (arr[j]->type.circle == NULL) {
    exit(1);
  }
  arr[j]->is_circle = 1;
  arr[j]->is_init = 1;
  arr[j]->type.circle->key1 = key1;
  arr[j]->type.circle->key2 = key2;
  return arr;
}
Regular **add_dot_to_keys_struct(Regular **arr, int keys_struct_index)
{
  int j = keys_struct_index;
  arr[j] = create_struct();
  arr[j]->is_dot = 1;
  arr[j]->is_init = 1;
  return arr;
}

int pass_second_circle_in_key(int i, const char *key_word)
{
  while (key_word[i] != ')') {
    i++;
  }
  i++;
  return i;
}

int count_number_of_backslash(const char *key, int index)
{
  int count = 1;
  while (index > 0) {
    if (key[index - 1] != BACK_SLASH) {
      return count;
    }
    count++;
    index--;
  }
  return count;
}

int check_if_even_number(int num)
{
  if (num % 2 == 0) {
    return 1;
  }
  return 0;
}

Regular **add_key_to_structs_list(char *key_word, Regular *arr[])
{
  int i = 0, j = 0;
  int len = strlen(key_word);
  while (i < len) {
    if (key_word[i] == '(') {
      if (i == 0 || key_word[i - 1] != BACK_SLASH) {
        add_circle_keys_to_struct(arr, j, key_word, i);
        j++;
        i = pass_second_circle_in_key(i, key_word);
      }
    } else if (key_word[i] == '[' && key_word[i - 1] != BACK_SLASH) {
      i++;
      add_squere_range_to_keys(arr, j, key_word, i);
      arr[j]->type.squere->start = key_word[i];
      i = i + 2;
      arr[j]->type.squere->end = key_word[i];
      i = i + 2;
      j++;
    } else if (key_word[i] == '.' && key_word[i - 1] != BACK_SLASH) {
      add_dot_to_keys_struct(arr, j);
      i++;
      j++;
    } else {  // normal char:
      if (key_word[i] != BACK_SLASH) {
        add_simple_char_to_keys(arr, j, key_word, i);
        i++;
        j++;
        continue;
      }
      // deal with '\' or '\\':
      int num_of_prev_back_slash = 0;
      num_of_prev_back_slash = count_number_of_backslash(key_word, i);
      if (check_if_even_number(num_of_prev_back_slash) == 1) {
        add_simple_char_to_keys(arr, j, key_word, i);
        i++;
        j++;
        continue;
      } else {
        i++;
      }
    }
  }
  arr[j] = create_struct();
  arr[j]->is_init = LAST_STRUCT;
  return arr;
}

int check_single_match_for_circle(char *line, Regular **keys, int line_index, int key_index, int sign)
{
  int i = line_index;
  int key_char_index = 0, match = 1;
  int line_len = strlen(line);
  // check if key1 match:
  if (sign == 1) {
    int key1_len = strlen(keys[key_index]->type.circle->key1);
    while (i < line_len && key_char_index < key1_len) {
      if (keys[key_index]->type.circle->key1[key_char_index] != line[i]) {
        match = 0;
        return match;
      }
      i++;
      key_char_index++;
    }
    // if match: return the number of indexes moved in line.
    return i;
  }
  // check for key2 match:
  if (sign == 2) {
    int key2_len = strlen(keys[key_index]->type.circle->key2);
    while (i < line_len && key_char_index < key2_len) {
      if (strcmp(keys[key_index]->type.circle->key2, " ") == 0) {
        return -1;
      }
      if (keys[key_index]->type.circle->key2[key_char_index] != line[i]) {
        match = 0;
        return match;
      }
      i++;
      key_char_index++;
    }
    // if match: return the number of line_index progressed
    return i;
  }
}

int progress_rec_line_index(int match, int line_index, int rec_line_index)
{
  if (match == -1) {
    return rec_line_index;
  }
  if (match == 0) {
    rec_line_index = line_index + 1;
  } else {
    rec_line_index++;
  }
  return rec_line_index;
}

int check_match_for_simple_str(const char *line, int line_index, const char *simple_key_str)
{
  int match = 0;
  if (line[line_index] == simple_key_str[0]) {
    match = 1;
  }
  return match;
}

void handle_i_arg(Flags *f_arguments, char *key) { upperToLower(key); }
int check_if_reg_match_rec(char *line, Regular **keys, int line_index, int key_index, int match_flag,
                           Flags *f_arguments)
{
  int line_lenght = strlen(line);
  while (line_index < (line_lenght) && keys[key_index]->is_init == 1) {
    if (keys[key_index]->is_simple == 1) {
      match_flag = check_match_for_simple_str(line, line_index, keys[key_index]->type.simple->simple_str);
      if (line_index == line_lenght - 1) {
        return match_flag;
      }
      if (match_flag == 0) {
        key_index = 0;
        line_index++;
      } else {
        line_index++;
        key_index++;
      }
      continue;
    }
    if (keys[key_index]->is_dot == 1) {
      if (keys[key_index + 1]->is_init != 1) {
        return match_flag;
      }
      line_index++;
      key_index++;
    }
    if (keys[key_index]->is_squere == 1) {
      if (line[line_index] >= keys[key_index]->type.squere->start &&
          line[line_index] <= keys[key_index]->type.squere->end) {
        match_flag = 1;
        line_index++;
        key_index++;
      } else {
        match_flag = 0;
        key_index = 0;
        line_index++;
      }
    }
    if (keys[key_index]->is_circle == 1) {
      int rec_match1 = 1, rec_match2 = 1;
      int rec_line_index1 = line_index;
      int rec_line_index2 = line_index;
      rec_match1 = check_single_match_for_circle(line, keys, line_index, key_index, 1);
      rec_match2 = check_single_match_for_circle(line, keys, line_index, key_index, 2);
      key_index++;
      rec_line_index1 = progress_rec_line_index(rec_match1, line_index, rec_line_index1);
      rec_line_index2 = progress_rec_line_index(rec_match2, line_index, rec_line_index2);
      match_flag =
          check_if_reg_match_rec(line, keys, rec_line_index1, key_index, rec_match1, f_arguments) * rec_match1 ||
          check_if_reg_match_rec(line, keys, rec_line_index2, key_index, rec_match2, f_arguments) * rec_match2;
      if (match_flag == 1) {
        return match_flag;
      }
      if (match_flag == 0) {
        key_index = 0;
        line_index++;
      }
    }
  }
  return match_flag;
}

void free_all_memory(Regular **keys)
{
  int i = 0, counter = 0;
  while (keys[i]->is_init != LAST_STRUCT) {
    if (keys[i]->is_circle == 1) {
      free(keys[i]->type.circle->key1);
      free(keys[i]->type.circle->key2);
      free(keys[i]->type.circle);
    }
    if (keys[i]->is_squere == 1) {
      free(keys[i]->type.squere);
    }
    if (keys[i]->is_simple == 1) {
      free(keys[i]->type.simple);
    }
    if (keys[i]->is_dot == 1) {
    }
    i++;
    counter++;
  }
  while (counter != -1) {
    free(keys[counter]);
    counter--;
  }
}

Regular **create_key_struct(const char *key_word, Regular **keys)
{
  int len = strlen(key_word);
  len++;
  char *key = (char *)malloc(sizeof(char) * strlen(key_word) + 1);
  if (key == NULL) {
    exit(1);
  }
  strcpy(key, key_word);
  add_key_to_structs_list(key, keys);
  free(key);
  return keys;
}

int check_match(Regular **keys, char *line_original, Flags *f_arguments)
{
  int flag = NONE_INIT_FLAG;
  int len = strlen(line_original);
  if (line_original[0] == '\n') {
    return 0;
  }
  char *line = (char *)malloc(sizeof(char) * len + 1);
  if (line == NULL) {
    exit(1);
  }
  strcpy(line, line_original);
  if (f_arguments->is_i == 1) {
    handle_i_arg(f_arguments, line);
  }
  flag = check_if_reg_match_rec(line, keys, 0, 0, 0, f_arguments);
  free(line);
  return flag;
}
