#include <stdlib.h>
#include <stdio.h>

void piranha_test_matrix(const float* f)
{
    int i;
    
    for (i = 0; i < 16; ++i)
        printf(" %f", f[i]);
    
    printf("\n");
    fflush(stdout);
}

void piranha_free(void* ptr)
{
    free(ptr);
}

const char* piranha_get_string()
{
    return "Greetings, from C!";
}

const char* piranha_get_null()
{
    return 0;
}
