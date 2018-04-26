.PHONY: all clean

include config.mk

all: $(OBJS)
	gcc $(CFLAGS) $(LIBS) main.c $(OBJS) -o $(PROJECT)

%: %.c
	gcc -c $(CFLAGS) $(LIBS) -o $@ $<

clean:
	rm $(PROJECT)
	rm $(OBJS)
