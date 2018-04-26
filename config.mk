PROJECT=rainbow-joe
LIBS=\
	`sdl2-config --libs`\
	-lSDL2_image\
	-lSDL2_mixer\
	`xml2-config --libs`
CFLAGS=\
	-pedantic-errors\
	-std=c99\
	-DWANT_ZLIB\
	-Isrc/tmx\
	-I/usr/include/libxml2\
	-Wall\
	-Werror\
	-Wextra\
	`sdl2-config --cflags`\
	`xml2-config --cflags`
SRCS=$(wildcard src/*.c) $(wildcard src/tmx/*.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
