#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "errors.h"

/*
 *This function will handle errors that occurres 
 *while managing files and prints the right message.
 */
void handle_files_error(const char *message, char *file_path) {
   char full_message[300];

   check_input_size(sizeof(full_message), (strlen(message)+strlen(file_path)), "errors.h -> handle_files_error()"); //Check the final full_message size
   strcpy(full_message, message);
   strcat(full_message, file_path);
   perror(full_message);
   exit(EXIT_FAILURE);
}

/*
 *This function will check if the size of the input is 
 *valid and can fit in the array without overflowing it.
 */
void check_input_size(int valid_size, int input_size, const char *error_location) {
   if(input_size < valid_size)
      return;
   fprintf(stderr, "%s %s %s\n", "The size of the inserted string in", error_location, "is invalid, please check the size of the inserted string and try again!");
   exit(EXIT_FAILURE);
}

/*
 *This function will handle general errors and display 
 *the relevant message.
 */
void handle_general_error(const char *message) {
   fprintf(stderr, "%s\n", message);
   exit(EXIT_FAILURE);
}

/*
 *This function will handle errors for the strstr() function 
 *and it will display the write message if a string wasnt found.
 */
void handle_strstr_error(const char *message, const char *serched_str) {
   fprintf(stderr, "%s %s\n", message, serched_str);
   exit(EXIT_FAILURE);
}
