TOOLCHAIN=
#TOOLCHAIN=i686-w64-mingw32
CC=$(TOOLCHAIN)-cc
PROJECT=rainbow-joe
#PROJECT=rainbow-joe.exe
LIBS=\
	-lSDL2 -lpthread\
	-lSDL2_image\
	-lSDL2_mixer\
	-lxml2 -lz -llzma -licuuc -lm
CFLAGS=\
	-L/usr/$(TOOLCHAIN)/lib\
	-I/usr/$(TOOLCHAIN)/include\
	-I/usr/$(TOOLCHAIN)/include/libxml2\
	-O2\
	-pedantic-errors\
	-std=c99\
	-DWANT_ZLIB\
	-Wall\
	-Werror\
	-Wextra\
	-D_REENTRANT
SRCS=\
	$(wildcard src/*.c)\
	$(wildcard src/tmx/*.c)\
	$(wildcard src/inih/*.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
