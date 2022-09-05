ifeq ($(OS),Windows_NT)
	CFLAGS += -DJAWBONE_OS_WINDOWS
endif

CFLAGS += -shared -fPIC -O3 -DNDEBUG
TARGET = PiranhaNative.dll
SOURCE = piranha.c piranha-stb.c stb_vorbis.c piranha-sqlite3.c 

ifeq ($(OS),Windows_NT)
	SOURCE += net/piranha-net-windows.c
	LDFLAGS += -static
	LDLIBS += -lws2_32
else
	SOURCE += net/piranha-net-linux.c
endif

all : $(TARGET)

$(TARGET) : $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean :
	rm -f -v $(TARGET)
