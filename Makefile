# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/26 11:03:39 by bfitte            #+#    #+#              #
#    Updated: 2026/02/27 18:55:00 by bfitte           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME := codexion

INCLUDE := coders/

BUILD_DIR := .build

RM := rm -f

CC := cc

FLAGS := -Wall -Werror -Wextra -pthread

FINCLUDE := -I $(INCLUDE)

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
	get_set3.c\

OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
DEPS := $(patsubst %.o, %.d, $(OBJ))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(FINCLUDE) $(OBJ) -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(FLAGS) $(FINCLUDE) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJ) $(DEPS)
	rm -rf $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
