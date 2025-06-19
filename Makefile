# **************************************************************************** #
#																			  #
#														 :::	  ::::::::	#
#	Makefile										   :+:	  :+:	:+:	#
#													 +:+ +:+		 +:+	  #
#	By: mpierce <mpierce@student.hive.fi>		  +#+  +:+	   +#+		 #
#												 +#+#+#+#+#+   +#+			#
#	Created: 2025/03/12 14:45:55 by clu			   #+#	#+#			  #
#	Updated: 2025/04/18 17:09:58 by mpierce		  ###   ########.fr		#
#																			  #
# **************************************************************************** #

# Program Name
NAME = minishell

# Comiler and Flags
CC 		= cc
CFLAGS 	= -Wall -Werror -Wextra

# Directories
SRC_DIR 		= ./src
EXEC_DIR		= execution/
EXP_DIR			= parse_input/expander/
LEX_DIR			= parse_input/lexer/
PAR_DIR			= parse_input/parser/
PARSE_DIR		= parse_input/
UTL_DIR			= utils/
BLT_DIR			= builtins/
OBJ_DIR 		= ./obj
LIBFT_DIR 		= ./libft

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a

# Header files
HEADERS = -I./include -I$(LIBFT_DIR)/include
HEADERS_DEP = include/minishell.h

# Source files and object files
SRC = \
	$(SRC_DIR)$(EXEC_DIR)file_handler.c $(SRC_DIR)$(EXEC_DIR)exec_cmds.c $(SRC_DIR)$(EXEC_DIR)exec_master.c \
	$(SRC_DIR)$(EXEC_DIR)exec_pipeline.c $(SRC_DIR)$(EXEC_DIR)execve.c $(SRC_DIR)$(EXEC_DIR)exec_utils.c \
	$(SRC_DIR)$(EXEC_DIR)heredoc_utils.c $(SRC_DIR)$(EXEC_DIR)only_redir.c $(SRC_DIR)$(EXP_DIR)expand.c \
	$(SRC_DIR)$(EXP_DIR)expand_utils.c $(SRC_DIR)$(LEX_DIR)operators.c \
	$(SRC_DIR)$(LEX_DIR)quotes.c $(SRC_DIR)$(LEX_DIR)lexer.c $(SRC_DIR)$(LEX_DIR)tokens.c \
	$(SRC_DIR)$(LEX_DIR)words.c $(SRC_DIR)$(PAR_DIR)ast.c $(SRC_DIR)$(EXEC_DIR)execve_utils.c \
	$(SRC_DIR)$(PAR_DIR)parser.c $(SRC_DIR)$(PARSE_DIR)process_input_utils.c \
	$(SRC_DIR)$(PARSE_DIR)process_input.c $(SRC_DIR)$(UTL_DIR)error_utils.c $(SRC_DIR)$(UTL_DIR)shlvl.c \
	$(SRC_DIR)$(UTL_DIR)utils.c $(SRC_DIR)$(UTL_DIR)prompt.c $(SRC_DIR)$(UTL_DIR)free.c $(SRC_DIR)$(UTL_DIR)free_utils.c \
	$(SRC_DIR)$(UTL_DIR)signals.c $(SRC_DIR)$(BLT_DIR)builtin_utils.c $(SRC_DIR)$(BLT_DIR)cd_utils.c \
	$(SRC_DIR)$(BLT_DIR)exit.c $(SRC_DIR)$(BLT_DIR)export_utils.c $(SRC_DIR)$(BLT_DIR)cd.c \
	$(SRC_DIR)$(BLT_DIR)echo.c $(SRC_DIR)$(BLT_DIR)export.c $(SRC_DIR)$(BLT_DIR)misc.c $(SRC_DIR)main.c \
	$(SRC_DIR)$(PAR_DIR)redirects.c $(SRC_DIR)$(PAR_DIR)validate.c $(SRC_DIR)$(EXEC_DIR)update_env.c \
	$(SRC_DIR)$(EXP_DIR)retoken.c $(SRC_DIR)$(PAR_DIR)redirects_utils.c $(SRC_DIR)$(PAR_DIR)parser_utils.c \
	$(SRC_DIR)$(BLT_DIR)extras.c 
OBJ = $(SRC:$(SRC_DIR)%=$(OBJ_DIR)/%)
OBJ := $(OBJ:.c=.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling $(NAME) ..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(HEADERS) -o $(NAME) -lreadline
	@echo "Done"

# Compile the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS_DEP)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Compile the Libft library
$(LIBFT):
	@echo "Compiling Libft..."
	@make -C $(LIBFT_DIR) --no-print-directory
	@echo "Done"

clean:
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@rm -rf $(OBJ_DIR)
	@echo "Cleaned up all object files"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "Cleaned up $(NAME)"
	@echo "Cleaned up everything"

re: fclean all
	@echo "Remade everything"

full: re
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@rm -rf $(OBJ_DIR)
	@echo "Cleaned up all object files"

.PHONY: all clean fclean re full
