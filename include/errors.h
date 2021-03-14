#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

/*
*This header is for handling errors 
*/

//Functions prototype
void handle_files_error(const char *, char *);
void check_input_size(int, int, const char*);
void handle_general_error(const char *);
void handle_strstr_error(const char *, const char *);

#endif
