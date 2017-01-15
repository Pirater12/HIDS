OBJS= main.o
DIRS=C:/mingw32/i686-w64-mingw32/lib
$(info Checking current platform)
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    $(info Current platform is $(UNAME). OK...)
    DIRS=/usr/include/SDL2
endif

ifeq ($(UNAME),Darwin)
    $(info Current platform is $(UNAME). OK...)
    DIRS=/usr/local/include/SDL2
    LIBS=-L/usr/local/lib
endif

all: input

input: $(OBJS) 
	gcc $(OBJS) $(LIBS) -lws2_32 -o HIDS -g
	rm main.o

%.o: %.c
	gcc -c $< -o $@ -g -I$(DIRS) 
clean:
	rm obj/*
