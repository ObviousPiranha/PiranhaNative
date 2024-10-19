ifeq ($(OS),Windows_NT)
	CFLAGS += -DJAWBONE_OS_WINDOWS
endif

SOURCE = jawbone.c jawbone-stb.c stb_vorbis.c jawbone-sqlite3.c
CFLAGS += -shared -fPIC -O3 -DNDEBUG

# https://stackoverflow.com/a/14777895/264712
ifeq ($(OS),Windows_NT)
	TARGET = JawboneNative.dll
	LDFLAGS += -static
	LDLIBS += -lws2_32
else
	uname_S := $(shell uname -s)
	ifeq ($(uname_S),Darwin)
		TARGET = libJawboneNative.dylib
	else
		TARGET = libJawboneNative.so
	endif
endif

all : $(TARGET)

$(TARGET) : $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean :
	rm -f -v $(TARGET)
