#include <stdlib.h>
#include <stdio.h>
#include "piranha-export.h"

JawboneExport void piranha_test_matrix(const float* f)
{
    int i;

    for (i = 0; i < 16; ++i)
        printf(" %f", f[i]);

    printf("\n");
    fflush(stdout);
}

JawboneExport void piranha_free(void* ptr)
{
    free(ptr);
}

JawboneExport const char* piranha_get_string()
{
    return "Greetings, from C!";
}

JawboneExport const char* piranha_get_null()
{
    return 0;
}
