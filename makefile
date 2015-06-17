TARGET =    	run
OBJS = main.o rate.o move.o
SRCS = $(OBJS:.o=.c)
CC =    	-c
$(TARGET): $(OBJS)
		gcc -Wall -lm -o $@ $^

.c.o:
		gcc -Wall $(CC) $<

dep : 
		gccmakedep $(SRCS)
