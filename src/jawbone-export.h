#ifndef PIRANHA_EXPORT_H
#define PIRANHA_EXPORT_H

#ifdef JawboneWindows
#define JawboneExport __declspec(dllexport)
#else
#define JawboneExport
#endif

#endif
