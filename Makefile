SRC := LogicReducerUI.c logicReadIn.c minterm_reducer.c
OBJ := $(SRC:.c=.o)

%.o: %.c
	$(CC) -c '$^' -o $@

logicReducer: $(OBJ)
	$(CC) $^ -o $@

.PHONY: clean

clean:
	rm -fv $(OBJ)
	rm -fv logicReducer
