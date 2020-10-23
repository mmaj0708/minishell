/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 15:19:57 by ncolomer          #+#    #+#             */
/*   Updated: 2019/10/25 01:15:57 by hcanon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_str
{
	char			*line;
	struct s_str	*next;
}				t_str;

typedef struct	s_fd
{
	int			fd;
	t_str		*str;
	struct s_fd	*next;
}				t_fd;

int				get_next_line(int fd, char **line);
char			*ft_strdup(char const *s1);
t_fd			*find_fd(t_fd **list, int fd, int *new);
int				check_lf(t_str *str, char *sim_str);
int				fill_in(t_str **str, char *buffer, int fd);
int				lst_clear(t_str **list);

#endif
