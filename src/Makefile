ifeq ($(OS),Windows_NT)
	CFLAGS += -static
endif

CFLAGS += -shared -fPIC -O3 -DSQLITE_OMIT_DEPRECATED=1 -DNDEBUG
TARGET = PiranhaNative.dll
SOURCE = piranha.c stb.c stb_vorbis.c sqlite3.c

all : $(TARGET)

$(TARGET) : $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean :
	rm -f -v *.o *.bin *.exe *.so *.dll *.lib *.exp *.dylib
