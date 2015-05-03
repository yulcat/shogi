TARGET =    	shogi
OBJS = main.o rate.o move.o
SRCS = $(OBJS:.o=.c)
CC =    	-c
$(TARGET): $(OBJS)
		gcc -lm -o $@ $^

.c.o:
		gcc $(CC) $<

dep : 
		gccmakedep $(SRCS)
