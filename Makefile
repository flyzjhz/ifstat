CFLAGS := -ggdb
INC := -I.
CC := i686-pc-linux-gnu-gcc

ifneq ($(V),)
	CFLAGS += -Wall
endif

all: ifstat

ifstat: ifstat.c
#	echo $(CC) $(CFLAGS) $(INC)
	$(CC) $(CFLAGS) $(INC) -o ifstat ifstat.c 
    
clean:
	rm -f ifstat

