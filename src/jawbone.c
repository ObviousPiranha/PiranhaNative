#include <stdlib.h>
#include <stdio.h>
#include "jawbone-export.h"

JawboneExport void jawbone_test_matrix(const float* f)
{
    int i;

    for (i = 0; i < 16; ++i)
        printf(" %f", f[i]);

    printf("\n");
    fflush(stdout);
}

JawboneExport void jawbone_free(void* ptr)
{
    free(ptr);
}

JawboneExport const char* jawbone_get_string()
{
    return "Greetings, from C!";
}

JawboneExport const char* jawbone_get_null()
{
    return 0;
}
