.PHONY: all clean

include config.mk

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROJECT)

%: %.c
	$(CC) -c $(CFLAGS) $(LIBS) -o $@ $<

clean:
	rm $(OBJS)
	rm $(PROJECT)
