#include "C:/Users/100TR/C_PRACTICE/projects/c_syntax_checker/syntax_checker/syntax_checker.h"
#include <stdio.h> 

const char* filename = "testfile.c";

int main(void) {
    size_t f_size = 0;
    char *f_buffer = read_file_to_buffer(filename, &f_size);
    
    handle_syntax_errors(f_buffer, f_size);      
    
    free(f_buffer);
    return 0;
}
