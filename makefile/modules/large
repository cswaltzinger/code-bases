


#directories
INC = ./inc
SRC = ./src
OBJ = ./obj
#DOC = ./doc
BIN = ./bin
DIRS = $(INC) $(SRC) $(OBJ) $(DOC) $(BIN)
SRC_DIRS = $(SRC) $(INC)


CC = gcc -I $(INC)

LIBS = -lm -lpthread
# -lpthread -lz -lpng -ljpeg -lSDL2 -lGL -lglfw

CFLAGS = -std=gnu99 -Wall -g  #-DDEBUG



CLEANERS =  *~ ./$(SRC)/*~ ./$(OBJ)/* ./$(BIN)/*
CLEANERS = $(CLEANERS)  *~ *.o *.dSYM 

#source file paths
vpath %.h $(INC)
vpath %.c $(SRC)
vpath %.o $(OBJ)


all: $(EXECS)

init: mkpaths

mkpaths:
	mkdir -p $(DIRS)

%.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@


dsym:
    /bin/rm -rf *.dSYM
clean: dsym
	/bin/rm -rf $(OBJ) $(EXECS)  $(CLEANERS)