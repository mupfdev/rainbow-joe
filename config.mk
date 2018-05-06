PROJECT=rainbow-joe
LIBS=\
	`sdl2-config --libs`\
	-lSDL2_image\
	-lSDL2_mixer\
	`xml2-config --libs`
CFLAGS=\
	-Isrc/inih\
	-Isrc/tmx\
	-I/usr/include/libxml2\
	-O2\
	-pedantic-errors\
	-std=c99\
	-DWANT_ZLIB\
	-Wall\
	-Werror\
	-Wextra\
	`sdl2-config --cflags`\
	`xml2-config --cflags`
SRCS=\
	$(wildcard src/*.c)\
	$(wildcard src/tmx/*.c)\
	$(wildcard src/inih/*.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
