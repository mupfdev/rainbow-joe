.PHONY: all clean

include config.mk

all: $(OBJS)
	gcc $(CFLAGS) $(LIBS) $(OBJS) -o $(PROJECT)

%: %.c
	gcc -c $(CFLAGS) $(INC) $(LIBS) -o $@ $<

clean:
	rm $(PROJECT)
	rm $(OBJS)
