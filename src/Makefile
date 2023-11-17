# $Id $

# Makefile for Rom24. Works fine on my Debian system.
# You may need to use 'gmake' on BSD systems.

COLOR = \x1B[36m
CC = gcc
RM = rm
EXE = arcanum
C_FLAGS = -Wall -pedantic -Wno-char-subscripts -Wno-unused-variable \
	  -Wno-pointer-sign -Wno-format-truncation
#C_FLAGS = -Wall
L_FLAGS = -lz -lpthread -lcrypt

# Source Files
SRC_FILES := $(wildcard *.c)

# Object Files
OBJ_DIR = obj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(OBJ_FILES)
	@$(CC) -o $(EXE) $(OBJ_FILES) $(L_FLAGS)
	@echo -e "$(COLOR)$(EXE) successfully compiled."

$(OBJ_DIR)/%.o: %.c
	@echo "Compiling $<"
	@$(CC) -c $(C_FLAGS) -o $@ $<

clean:
	@rm -f $(EXE)
	@rm -f $(OBJ_FILES) *~ *.bak *.orig *.rej
	@echo "Arcanum source files cleaned"
