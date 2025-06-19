# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 15:08:55 by clu               #+#    #+#              #
#    Updated: 2025/04/18 13:08:11 by mpierce          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = libft.a

# Compiler and Flags
CC = cc
CFLAGS = -Wall -Werror -Wextra -g

# Directories
SRC_DIR	= ./src
OBJ_DIR	= ./obj

# Header
HEADER = -I./include

# Source files and object files
SRC = $(SRC_DIR)/ft_add_to_array.c $(SRC_DIR)/ft_atoi.c $(SRC_DIR)/ft_atol.c $(SRC_DIR)/ft_atoll.c $(SRC_DIR)/ft_bzero.c $(SRC_DIR)/ft_calloc.c $(SRC_DIR)/ft_format_check.c \
	$(SRC_DIR)/ft_free_array.c $(SRC_DIR)/ft_isalnum.c $(SRC_DIR)/ft_isalpha.c $(SRC_DIR)/ft_isascii.c $(SRC_DIR)/ft_isdigit.c $(SRC_DIR)/ft_isprint.c \
	$(SRC_DIR)/ft_is_whitespace.c $(SRC_DIR)/ft_itoa.c $(SRC_DIR)/ft_lstadd_back_bonus.c $(SRC_DIR)/ft_lstadd_front_bonus.c $(SRC_DIR)/ft_lstclear_bonus.c \
	$(SRC_DIR)/ft_lstdelone_bonus.c $(SRC_DIR)/ft_lstiter_bonus.c $(SRC_DIR)/ft_lstlast_bonus.c $(SRC_DIR)/ft_lstmap_bonus.c $(SRC_DIR)/ft_lstnew_bonus.c \
	$(SRC_DIR)/ft_lstsize_bonus.c $(SRC_DIR)/ft_memchr.c $(SRC_DIR)/ft_memcmp.c $(SRC_DIR)/ft_memcpy.c $(SRC_DIR)/ft_memmove.c $(SRC_DIR)/ft_memset.c $(SRC_DIR)/ft_printf.c \
	$(SRC_DIR)/ft_putchar.c $(SRC_DIR)/ft_putchar_fd.c $(SRC_DIR)/ft_putendl_fd.c $(SRC_DIR)/ft_puthex.c $(SRC_DIR)/ft_puthex_low.c $(SRC_DIR)/ft_puthex_up.c $(SRC_DIR)/ft_putnbr.c \
	$(SRC_DIR)/ft_putnbr_fd.c $(SRC_DIR)/ft_putnbr_i.c $(SRC_DIR)/ft_putptr.c $(SRC_DIR)/ft_putstr.c $(SRC_DIR)/ft_putstr_fd.c $(SRC_DIR)/ft_realloc.c $(SRC_DIR)/ft_split.c \
	$(SRC_DIR)/ft_strchr.c $(SRC_DIR)/ft_strcmp.c $(SRC_DIR)/ft_strdup.c $(SRC_DIR)/ft_striteri.c $(SRC_DIR)/ft_strjoin.c $(SRC_DIR)/ft_strlcat.c $(SRC_DIR)/ft_strlcpy.c \
	$(SRC_DIR)/ft_strlen.c $(SRC_DIR)/ft_strmapi.c $(SRC_DIR)/ft_strncmp.c $(SRC_DIR)/ft_strndup.c $(SRC_DIR)/ft_strnstr.c $(SRC_DIR)/ft_strrchr.c $(SRC_DIR)/ft_strtrim.c \
	$(SRC_DIR)/ft_substr.c $(SRC_DIR)/ft_swap.c $(SRC_DIR)/ft_tolower.c $(SRC_DIR)/ft_toupper.c $(SRC_DIR)/get_next_line.c 

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Creating static library $(NAME)"
	@ar rcs $(NAME) $(OBJ)
	@echo "Done"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean:
	@echo "Cleaning libft object files"
	@rm -f $(OBJ)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleanining $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
