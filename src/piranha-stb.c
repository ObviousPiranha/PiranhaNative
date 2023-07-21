#ifdef JawboneWindows
#define STBIDEF __declspec(dllexport)
#define STBIWDEF __declspec(dllexport)
//#define STBTT_DEF __declspec(dllexport)
#endif

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
