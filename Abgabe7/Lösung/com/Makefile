SRC    = communication.c configureSerial.c buffer.c # source files
CFLAGS = -Wall
LFLAGS = 
OBJ    = $(SRC:.c=.o) # object files

# rule to compile C files
.c.o:
	gcc -v -v -v -c $(CFLAGS) $< -o $@

comm: $(OBJ)
	gcc $(LFLAGS) $(OBJ) -o $@

clean:
	rm $(OBJ)
