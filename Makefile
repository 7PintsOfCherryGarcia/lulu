CC=gcc
CFLAGS= -Wall -g -std=gnu11 -pedantic
INC= -I.

PROG= lulu
OBJS= main.o
SRC=src

LIB_JellyBelly= -lm -lz
#LIB_create_spaces= -lm
.PHONY:all clean
.SUFFIXES:.c .o

%.o:$(SRC)/%.c
	$(CC) -c $(CFLAGS) $(INC) $< -o $@


all:$(PROG) $(PROG_EXTRA)


lulu:$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIB_JellyBelly)

#create_spaces:$(OBJS_EXTRA)
#	$(CC) $(CFLAGS) -o $@ $(OBJS_EXTRA) $(LIB_create_spaces)

clean:
	rm -rf $(OBJS) $(OBJS_EXTRA)


# makedepend line not in use in current compilation enviroanment

