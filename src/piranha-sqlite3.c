#include "piranha-export.h"

// https://www.sqlite.org/compile.html

#define SQLITE_API JawboneExport
#define SQLITE_OMIT_AUTOINIT 1
#define SQLITE_OMIT_DECLTYPE 1
#define SQLITE_OMIT_DEPRECATED 1
#include "sqlite3.c"
