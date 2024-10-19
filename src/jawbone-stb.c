#include "jawbone-export.h"

#define STBIDEF JawboneExport
#define STBIWDEF JawboneExport
//#define STBTT_DEF JawboneExport

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
