SHELL := /bin/bash

NAME := modular
SRC_DIR := src
OBJ_DIR := obj
MLX42_DIR := MLX42
MLX42 := $(MLX42_DIR)/build/libmlx42.a
HEADERS := $(shell find include -type f -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SOURCES:.c=.o))

CC  := cc
IFLAGS := -Iinclude -I$(MLX42_DIR)/include
CFLAGS := 
#-Wall -Wextra -Werror
LFLAGS := -L$(MLX42_DIR)/build -lmlx42 -lglfw -ldl -pthread -lm

_DEBUG := 0
ifeq ($(_DEBUG),1)
	CFLAGS += -g3 -fsanitize=address
endif

all: $(MLX42) $(NAME)

$(MLX42):
	cmake $(MLX42_DIR) -B $(MLX42_DIR)/build
	$(MAKE) -C $(MLX42_DIR)/build -j4 --quiet

$(NAME): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $(NAME) 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(SIZE_FLAGS) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	$(MAKE) clean -C $(MLX42_DIR)/build -j4 --quiet
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) clean/fast -C $(MLX42_DIR)/build -j4 --quiet
	-rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
