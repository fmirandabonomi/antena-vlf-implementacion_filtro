.SUFFIXES:

PROGNAME = imp_filtro
SRC_DIR = src
INC_DIRS = inc
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
LD = gcc
LFLAGS = 

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(basename $(notdir $(SRC_FILES)))))
DEP_FILES = $(addsuffix .d,$(basename $(OBJ_FILES)))
PROG = $(BIN_DIR)/$(PROGNAME)

all: $(PROG)

$(PROG): $(OBJ_FILES) | $(BIN_DIR)
	$(LD) $(LFLAGS) -o $(PROG) $(OBJ_FILES)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(foreach d,$(INC_DIRS),-I$(d)) -c -MMD -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEP_FILES)
