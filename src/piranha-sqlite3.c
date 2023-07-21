// https://www.sqlite.org/compile.html

#ifdef JawboneWindows
#define SQLITE_API __declspec(dllexport)
#endif

#define SQLITE_OMIT_AUTOINIT 1
#define SQLITE_OMIT_DECLTYPE 1
#define SQLITE_OMIT_DEPRECATED 1
#include "sqlite3.c"
