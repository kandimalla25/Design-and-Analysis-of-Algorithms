/*	This program is for finding out the longest subsequence of the two given sequences	
	Developed by									
	Kandimalla, Devi Sri Haritha Lakshmi Pravallika 
	1000233072
	Date of development and submission - March 2022
*/

#include <stdio.h>
#include <stdlib.h>

// https://stackoverflow.com/questions/3437404/min-and-max-in-c
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAX_SUBSEQUENCE_LENGTH 25000
#define NUM_ALPHABETS 256

struct common_seq_arr_ptrs
{
  int *first_seq_arr;
  int *second_seq_arr;
  int size_first_seq_arr;
  int size_second_seq_arr;
};

struct lsis_subsequence
{
  int *sub_seq_arr;
  int size_sub_seq_arr;
  double cpu_time;
};

struct common_seq_arr_ptrs read_input_and_print(char input_file_name[])
{
  FILE *fptr;
  fptr = fopen(input_file_name, "r");
  if (fptr == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  struct common_seq_arr_ptrs input_arr_struct;
  int *combined_seq_arr;
  int NUM_ARRAYS = 2;
  int size_arr[NUM_ARRAYS];
  int total_size = 0;
  int integers_read_so_far = 0;
  int current_integer;
  int combined_seq_arr_index = 0;
  int allocated_mem = 0; // false initially

  while (fscanf(fptr, "%d", &current_integer) == 1)
  {
    integers_read_so_far++;
    if (integers_read_so_far <= NUM_ARRAYS)
    {
      // This is the size of the of array num: 'size_arr_index' that needs to be allocated
      size_arr[integers_read_so_far - 1] = current_integer;
      total_size += current_integer;
    }
    else if (current_integer != -1)
    {
      if (!allocated_mem)
      {
        combined_seq_arr = malloc(total_size * sizeof(int));
        allocated_mem = 1;
      }
      combined_seq_arr[combined_seq_arr_index] = current_integer;
      combined_seq_arr_index++;
    }
  }

  // now allocate sizes based on what was read previously
  input_arr_struct.first_seq_arr = malloc(size_arr[0] * sizeof(int));
  input_arr_struct.second_seq_arr = malloc(size_arr[1] * sizeof(int));
  input_arr_struct.size_first_seq_arr = size_arr[0];
  input_arr_struct.size_second_seq_arr = size_arr[1];

  // print the combined values
  int i;
  for (i = 0; i < total_size; i++)
  {
    if (i < size_arr[0])
    {
      input_arr_struct.first_seq_arr[i] = combined_seq_arr[i];
    }
    else
    {
      input_arr_struct.second_seq_arr[i - size_arr[0]] = combined_seq_arr[i];
    }
  }

  return input_arr_struct;
}

struct lsis_subsequence lsis(int *s, int sequence_length)
{
  struct lsis_subsequence sub_sequence_struct;
  int outputsubsequence[sequence_length];
  int temp[sequence_length], index_array[sequence_length];
  int tempseqlen;
  int i, j;
  int lsis_len = 0;
  int lsis_position = 0;

  for (i = 0; i < sequence_length; i++)
  {
    temp[i] = 1;
    index_array[i] = 0;
    outputsubsequence[i] = -1;
  }

  for (i = 1; i < sequence_length; i++)
    for (j = 0; j < i; j++)
    {
      tempseqlen = temp[i];
      if (s[j] < s[i])
        temp[i] = MAX(temp[i], 1 + temp[j]);
      if (tempseqlen != temp[i])
        index_array[i] = j;
    }

  /* scan T[i] to find LSIS length and where LSIS length is occuring in T[] */
  for (i = 0; i < sequence_length; i++)
  {
    if (temp[i] > lsis_len)
    {
      lsis_len = temp[i];
      lsis_position = i;
    }
  }

  /* outputsubsequence */
  int temp_lsis_len = lsis_len;
  while (temp_lsis_len)
  {
    outputsubsequence[temp_lsis_len - 1] = s[lsis_position];
    lsis_position = index_array[lsis_position];
    temp_lsis_len--;
  }


  sub_sequence_struct.sub_seq_arr = malloc(lsis_len * sizeof(int));
  sub_sequence_struct.size_sub_seq_arr = lsis_len;
  for (i = 0; i < lsis_len; i++) {
    sub_sequence_struct.sub_seq_arr[i] = outputsubsequence[i];
  }
  printf("\n");

  return sub_sequence_struct;
}

struct lsis_subsequence lcs_dynamic_programming(struct common_seq_arr_ptrs input_arr_struct)
{
 
  struct lsis_subsequence sub_sequence_struct;
  int longest_common_subsequence[MAX_SUBSEQUENCE_LENGTH]; // TODO: make this dynamic
  int size_common_subsequence = 0;
  int dp_arr_rows = input_arr_struct.size_first_seq_arr + 1;
  int dp_arr_columns = input_arr_struct.size_second_seq_arr + 1;

 
  // for the DP method, we need an array that's one larger in size length & breadth wise
  int *dynamic_arr_ptr[dp_arr_rows];
  int i,j;
  for (i = 0; i < dp_arr_rows; i++)
  {
    dynamic_arr_ptr[i] = (int *)malloc(dp_arr_columns * sizeof(int));
  }

  // initialize the DP array with all 0s initially
  for (i = 0; i < dp_arr_rows; i++)
  {
    for (j = 0; j < dp_arr_columns; j++)
    {
      dynamic_arr_ptr[i][j] = 0;
    }
  }

  // now the DP algorithm cycles through the entire string
  for (i = 1; i < dp_arr_rows; i++)
  {
    for (j = 1; j < dp_arr_columns; j++)
    {
      // Note: there's a -1 in the input_arr_struct for the 2 arrays because they are 0 index & the DP array starts from 1
      if (input_arr_struct.first_seq_arr[i - 1] == input_arr_struct.second_seq_arr[j - 1])
      {
        dynamic_arr_ptr[i][j] = 1 + dynamic_arr_ptr[i - 1][j - 1];
       }
      else
      {
        dynamic_arr_ptr[i][j] = MAX(dynamic_arr_ptr[i - 1][j], dynamic_arr_ptr[i][j - 1]);
      }
    }
  }

  
  // now we need to traverse DP array to find out longest subsequence (start from last corner in the bottom right)
  int current_dp_row = input_arr_struct.size_first_seq_arr;
  int current_dp_column = input_arr_struct.size_second_seq_arr;
  int max_checks = dp_arr_rows + dp_arr_columns + 1;
  int current_checks = 0;
  int lcs_ptr = 0;

  while (dynamic_arr_ptr[current_dp_row][current_dp_column] != 0)
  {
    if (dynamic_arr_ptr[current_dp_row][current_dp_column] == dynamic_arr_ptr[current_dp_row - 1][current_dp_column])
    {
      current_checks++;
      current_dp_row--;
    }
    else if (dynamic_arr_ptr[current_dp_row][current_dp_column] == dynamic_arr_ptr[current_dp_row][current_dp_column - 1])
    {
      current_checks++;
      current_dp_column--;
    }
    else if (dynamic_arr_ptr[current_dp_row][current_dp_column] == (dynamic_arr_ptr[current_dp_row - 1][current_dp_column - 1] + 1))
    {
      current_checks++;
      longest_common_subsequence[lcs_ptr] = input_arr_struct.second_seq_arr[current_dp_column - 1];
      lcs_ptr++;
      current_dp_row--;
      current_dp_column--;
    }
  }

  sub_sequence_struct.sub_seq_arr = malloc(lcs_ptr * sizeof(int));
  sub_sequence_struct.size_sub_seq_arr = lcs_ptr;
  for (i = 0; i < lcs_ptr; i++) {
    sub_sequence_struct.sub_seq_arr[i] = longest_common_subsequence[lcs_ptr - i - 1];
  }

  return sub_sequence_struct;
}

struct lsis_subsequence lcs_lsis(struct common_seq_arr_ptrs input_arr_struct)
{
  int *position_arr[NUM_ALPHABETS];
  struct lsis_subsequence sub_sequence_struct;
  int i;
  for (i = 0; i < NUM_ALPHABETS; i++)
  {
    position_arr[i] = (int *)malloc(input_arr_struct.size_second_seq_arr * sizeof(int));
  }

  // initialize the position array with all -1s initially
  int j;
  for (i = 0; i < NUM_ALPHABETS; i++)
  {
    for (j = 0; j < input_arr_struct.size_second_seq_arr; j++)
    {
      position_arr[i][j] = -1;
    }
  }

  for (j = input_arr_struct.size_second_seq_arr - 1; j >= 0; j--)
  {
    // find column where index for second sequence number will be stored
    int k = 0;
    int current_entry = input_arr_struct.second_seq_arr[j];
    while (position_arr[current_entry][k] != -1)
    {
      k++;
    }
    position_arr[current_entry][k] = j;
  }

  // now store the indexes of second sequence in wrt first sequence
  int first_seq_indexes[input_arr_struct.size_first_seq_arr * input_arr_struct.size_first_seq_arr];
  // initialize the position array with all -1s initially
  for (i = 0; i < input_arr_struct.size_first_seq_arr * input_arr_struct.size_first_seq_arr; i++)
  {
    first_seq_indexes[i] = -1;
  }
  // now store all the indexes of second sequence wrt first
  int current_entry_index = 0;
  for (i = 0; i < input_arr_struct.size_first_seq_arr; i++)
  {
    int k = 0;
    while (position_arr[input_arr_struct.first_seq_arr[i]][k] != -1)
    {
      first_seq_indexes[current_entry_index] = position_arr[input_arr_struct.first_seq_arr[i]][k];
      k++;
      current_entry_index++;
    }
  }

  // now trim all the -1s from the indexes array
  int indexes_arr[current_entry_index];
  for (i = 0; i < current_entry_index; i++)
  {
    indexes_arr[i] = first_seq_indexes[i];
  }

  sub_sequence_struct = lsis(indexes_arr, current_entry_index);
  struct lsis_subsequence sub_sequence_struct_final;

  // now print the result in reverse
  sub_sequence_struct_final.sub_seq_arr = malloc(sub_sequence_struct.size_sub_seq_arr * sizeof(int));
  sub_sequence_struct_final.size_sub_seq_arr = sub_sequence_struct.size_sub_seq_arr;

  for (i = 0; i < sub_sequence_struct.size_sub_seq_arr; i++)
  {
    sub_sequence_struct_final.sub_seq_arr[i] = input_arr_struct.second_seq_arr[sub_sequence_struct.sub_seq_arr[i]];
  }

  return sub_sequence_struct_final;
}

int main()
{
  char input_file_name[] = "input-file.txt";
  int validation_failure = 0;
  struct common_seq_arr_ptrs input_arr_struct = read_input_and_print(input_file_name);

  // TODO: handle edge cases
  struct lsis_subsequence v1 = lcs_dynamic_programming(input_arr_struct);
  struct lsis_subsequence v2 = lcs_lsis(input_arr_struct);

  printf("%d %d\n", v1.size_sub_seq_arr, v2.size_sub_seq_arr);

  int i;
  if (v1.size_sub_seq_arr != v2.size_sub_seq_arr) {
    validation_failure = 1;
  } else if (v1.size_sub_seq_arr == v2.size_sub_seq_arr) {
    for (i = 0; i < v1.size_sub_seq_arr; i++)
    {
      if (v1.sub_seq_arr[i] != v2.sub_seq_arr[i]) {
        validation_failure = 1;
      }
    }
  }

  if (!validation_failure) {
    for (i = 0; i < v1.size_sub_seq_arr; i++)
    {
      printf("%d\n", v1.sub_seq_arr[i]);
    }
    printf("-1\n");
   
  }

   return 0;
}