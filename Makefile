# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/17 00:56:42 by hcanon            #+#    #+#              #
#    Updated: 2020/10/15 12:06:37 by mmaj             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#====================================OBJS=======================================
OBJS_GEN		=	${SRCS_GEN:.c=.o}
OBJS_BUILD		=	${SRCS_BUILD:.c=.o}
OBJS_GNL		=	${SRCS_GNL:.c=.o}
OBJS_ERR		=	${SRCS_ERR:.c=.o}
OBJS_REDIR		=	${SRCS_REDIR:.c=.o}


#====================================SRCS=======================================
SRCS_GNL		=	srcs/gnl/get_next_line.c		\
					srcs/gnl/get_next_line_utils.c
SRCS_GEN		=	srcs/general/main.c				\
					srcs/general/exec.c				\
					srcs/general/functions.c		\
					srcs/general/other_functions.c  \
					srcs/general/get_proper_arg.c
SRCS_BUILD		=	srcs/builds/echo.c				\
					srcs/builds/env.c				\
					srcs/builds/direction.c			\
					srcs/builds/export.c			\
					srcs/builds/unset.c				\
					srcs/builds/exit.c
SRCS_ERR		=	srcs/error_handling/parsing_error.c \
					srcs/error_handling/ctrl_error.c
SRCS_REDIR		=	srcs/redirections/redirect.c	\
					srcs/redirections/redirect_inf.c	\
					srcs/redirections/pipeline.c


#=============================FLAGS AND COMPILERS===============================
CC				=	gcc -g3
CFLAGS			=	-Wall -Wextra -Werror -I ${HEADER}
CFLAGS_TEST		=	-g3 -fsanitize=address
NAME			=	minishell
HEADER			=	./includes/


#================================AVAILABLE RULES================================
$(NAME):	make_libs test

#-----
make_libs:
		$(MAKE) all -C ./libft/

#-----
all:		$(NAME)

#-----
test:		${OBJS_GNL} ${OBJS_GEN} ${OBJS_BUILD} ${OBJS_ERR} ${OBJS_REDIR}
		${CC} ${CFLAGS} -o ${NAME} ${OBJS_GEN} ${OBJS_BUILD} ${OBJS_GNL} ${OBJS_ERR} ${OBJS_REDIR} ./libft/libft.a

#-----
clean:
		$(MAKE) clean -C ./libft/
		rm -f ${OBJS_GEN} ${OBJS_GNL} ${OBJS_BUILD} ${OBJS_ERR} ${OBJS_REDIR}

#-----
fclean:		clean
		$(MAKE) fclean -C ./libft/
		rm -f ${NAME}

#-----
re:		fclean ${NAME}

#-----
.PHONY:		all clean fclean re test make_libs
