CC=gcc-8
CFLAGS=-g -mfpu=neon-vfpv4 -mfloat-abi=hard -Wall
LD=gcc-8
LDFLAGS=
LIBS=-lm

PROG=rkrun
PROGSRC=$(PROG).c
PROGOBJ=$(PROG).o
SRC=vector.c rk.c
HEAD=$(SRC:.c=.h) env.h
OBJ=$(SRC:.c=.o)

all: $(PROG)

$(PROG): $(PROGOBJ) $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

$(PROGOBJ): $(PROGSRC) $(HEAD)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c %.h env.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f $(PROG)

.PHONY: all clean

