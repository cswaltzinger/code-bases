


CC = gcc -I $(INC)

LIBS = -lm -lpthread
# -lpthread -lz -lpng -ljpeg -lSDL2 -lGL -lglfw

CFLAGS = -std=gnu99 -Wall -g  #-DDEBUG


all: $(EXECS)

%.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@

%: %.o 
	$(CC) $< -o $@ $(LIBS)

clean: 
	/bin/rm -rf  *~ *.o *.dSYM