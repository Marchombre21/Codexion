# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/26 11:03:39 by bfitte            #+#    #+#              #
#    Updated: 2026/02/26 16:55:20 by bfitte           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME := codexion

INCLUDE := coders/

BUILD_DIR := .build

RM := rm -f

CC := cc

FLAGS := -Wall -Werror -Wextra -pthread

FINCLUDE := -I $(INCLUDE)

TFLAG := -fsanitize=thread

AFLAG := -fsanitize=address

GFLAG := -g3

DEPFLAGS := -MMD -MP

SRC_FILES := actions.c\
	create_pointers.c\
	free_all.c\
	main.c\
	parsing.c\
	utils.c\
	priority_utils.c\
	routines.c\
	creates_utils.c\
	actions_utils.c\
	get_set.c\
	get_set2.c\

OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
DEPS := $(patsubst %.o, %.d, $(OBJ))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(FINCLUDE) $(OBJ) -o $(NAME)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(FLAGS) $(FINCLUDE) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

thread: fclean $(OBJ)
	$(CC) $(FLAGS) $(TFLAG) $(FINCLUDE) $(OBJ) -o $(NAME)

address: fclean $(OBJ)
	$(CC) $(FLAGS) $(AFLAG) $(FINCLUDE) $(OBJ) -o $(NAME)

val: fclean $(OBJ)
	$(CC) $(FLAGS) $(GFLAG) $(FINCLUDE) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ) $(DEPS)
	rm -rf $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re thread address val

-include $(DEPS)
