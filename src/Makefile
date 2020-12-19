ifeq ($(OS),Windows_NT)
	CFLAGS += -static
endif

CFLAGS += -shared -fPIC -O3 -DNDEBUG
TARGET = PiranhaNative.dll
SOURCE = piranha.c piranha-stb.c stb_vorbis.c piranha-sqlite3.c

all : $(TARGET)

$(TARGET) : $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean :
	rm -f -v $(TARGET)
