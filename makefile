CC=gcc
CFLAGS= -g -Wall -ansi -pedantic

SRCS=addressing.c output.c assembler.c ast.c ast_utils.c data.c encoding.c first_pass.c handle_parsing.c macroTable.c parsing_utils.c preprocessor.c second_pass.c symbol_table.c utils.c validate.c
OBJS=$(SRCS:.c=.o)
DEPS=addressing.h output.h assembler.h ast.h data.h encoding.h first_pass.h handle_parsing.h macroTable.h parsing_utils.h preprocessor.h second_pass.h symbol.h symbol_table.h utils.h validate.h

TARGET=run

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -g

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
