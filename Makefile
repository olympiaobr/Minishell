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

NAME    =   minishell
CC      =   cc
CFLAGS  =   -Wall -Werror -Wextra -g
LIBS    =    Libft/libft.a

SRCS	=	main.c lexer.c \
			lexer_utils1.c lexer_utils2.c \
			lexer_utils3.c expansion.c expansion2.c\
			error_synthax1.c\
			error_synthax2.c error_synthax3.c\
			parsing.c parsing2.c parsing3.c\
			heredoc.c\
			parsing_utils1.c parsing_utils2.c parsing4.c\
			parsing5.c lexer_utils5.c lexer_utils6.c\
			path.c execution.c execution2.c execution3.c execution4.c\
			execution5.c execution6.c execution7.c execution8.c\
			execution9.c execution10.c\
			builtins1.c builtins2.c builtins3.c builtins4.c builtins5.c\
			utils1.c utils2.c signals.c lexer_utils4.c\
			utils3.c\

OBJS	=	$(SRCS:.c=.o)

all:		$(NAME)

$(NAME): $(OBJS)
			make -C Libft
			$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) -lreadline

clean:
			make clean -C Libft
			rm -rf $(OBJS)

fclean:		clean
			rm -rf $(NAME) $(LIBS)

re:			fclean all

.PHONY: all clean fclean re
