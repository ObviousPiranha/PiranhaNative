#include <stdlib.h>
#include <stdio.h>

__declspec(dllexport) void piranha_test_matrix(const float* f)
{
    int i;

    for (i = 0; i < 16; ++i)
        printf(" %f", f[i]);

    printf("\n");
    fflush(stdout);
}

__declspec(dllexport) void piranha_free(void* ptr)
{
    free(ptr);
}

__declspec(dllexport) const char* piranha_get_string()
{
    return "Greetings, from C!";
}

__declspec(dllexport) const char* piranha_get_null()
{
    return 0;
}
