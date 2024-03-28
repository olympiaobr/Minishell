# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 10:49:49 by jasnguye          #+#    #+#              #
#    Updated: 2024/03/27 11:09:19 by jasnguye         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    =   minishell
CC      =   cc
CFLAGS  =   -Wall -Werror -Wextra -Imlx -g
LIBS    =    Libft/libft.a

SRCS	=	main.c lexer.c \
			lexer_utils1.c lexer_utils2.c \
			lexer_utils3.c \
			tokenization.c
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
