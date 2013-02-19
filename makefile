#Current make system
BIN=./bin/
SOURCE=./src/


PROG=init getty sh
LIST=$(addprefix $(BIN)/, $(PROG))

all: $(LIST)

$(BIN)/%:  $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)
clean :
	-rm $(LIST)
    