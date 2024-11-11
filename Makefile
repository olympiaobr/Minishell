# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 10:49:49 by jasnguye          #+#    #+#              #
#    Updated: 2024/05/13 11:10:37 by jasnguye         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Werror -Wextra -g
LIBS    = Libft/libft.a
INCLUDE = -Iincludes -ILibft/includes

SRC_DIR = src/
SRC_FILES = main.c \
            lexer.c lexer_utils1.c lexer_utils2.c lexer_utils3.c lexer_utils4.c lexer_utils5.c lexer_utils6.c \
            error_synthax1.c error_synthax2.c error_synthax3.c \
            parsing.c parsing2.c parsing3.c parsing4.c parsing5.c \
            parsing_utils1.c parsing_utils2.c \
            expansion.c expansion2.c \
            execution.c execution2.c execution3.c execution4.c execution5.c \
            execution6.c execution7.c execution8.c execution9.c execution10.c \
            builtins1.c builtins2.c builtins3.c builtins4.c builtins5.c \
            utils1.c utils2.c utils3.c \
            signals.c \
            heredoc.c heredoc2.c \
            path.c

SRCS = $(addprefix $(SRC_DIR),$(SRC_FILES))
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make -C Libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	make clean -C Libft
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
