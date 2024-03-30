# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/16 16:03:18 by jasnguye          #+#    #+#              #
#    Updated: 2024/03/30 13:40:02 by jasnguye         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = ft_isdigit.c ft_isprint.c \
ft_memcpy.c ft_memmove.c \
ft_memset.c ft_strlen.c \
ft_strlcpy.c ft_strlcat.c \
ft_isalpha.c ft_isalnum.c \
ft_isascii.c ft_bzero.c \
ft_toupper.c ft_tolower.c \
ft_strchr.c ft_strrchr.c \
ft_strncmp.c ft_memchr.c \
ft_memcmp.c ft_atoi.c \
ft_strnstr.c ft_calloc.c \
ft_strdup.c ft_substr.c \
ft_strjoin.c ft_strtrim.c \
ft_split.c ft_itoa.c \
ft_strmapi.c ft_striteri.c \
ft_putchar_fd.c ft_putstr_fd.c \
ft_putendl_fd.c ft_putnbr_fd.c \
ft_printf.c helper_functions.c	\
get_next_line_utils.c get_next_line.c \
ft_lstnew.c ft_lstadd_back.c \
ft_lstsize.c ft_lstlast.c \
ft_putchar.c 

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror 
AR = ar rcs

NAME = libft.a

all: $(NAME)
$(NAME) : $(OBJS)
	$(AR) $(NAME) $(OBJS) 

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME) $(LIBS)

re: fclean all

.PHONY: all clean fclean re









