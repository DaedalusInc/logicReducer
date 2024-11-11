SRC := LogicReducerUI.c logicReadIn.c minterm_reducer.c exec.c
OBJ := $(SRC:.c=.o)

%.o: %.c
	$(CC) -g -c '$^' -o $@

logicReducer: $(OBJ)
	$(CC) -g $^ -o $@

.PHONY: clean

clean:
	rm -fv $(OBJ)
	rm -fv logicReducer
