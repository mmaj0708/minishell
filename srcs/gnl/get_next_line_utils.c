/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcanon <hcanon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:00:46 by hcanon            #+#    #+#             */
/*   Updated: 2020/09/04 14:58:29 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		lst_clear(t_str **list)
{
	t_str	*tmp;

	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->line);
		free(*list);
		(*list) = tmp;
	}
	*list = NULL;
	return (0);
}

char	*ft_strdup(char const *s1)
{
	char	*str;
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i])
		i++;
	str = (char*)malloc(sizeof(*str) * (i + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	str[i] = 0;
	return (str);
}

t_fd	*find_fd(t_fd **list, int fd, int *new)
{
	t_fd	*current_fd;
	t_fd	*first;

	current_fd = NULL;
	first = *list;
	*new = 0;
	while (*list && !current_fd)
	{
		if ((*list)->fd == fd)
			current_fd = *list;
		*list = (*list)->next;
	}
	*list = first;
	if (!current_fd)
	{
		if (!(current_fd = (t_fd *)malloc(sizeof(*current_fd))))
			return (NULL);
		current_fd->fd = fd;
		current_fd->next = *list;
		current_fd->str = NULL;
		*list = current_fd;
		*new = 1;
	}
	return (current_fd);
}

int		check_lf(t_str *str, char *newstr)
{
	int		i;

	if (!str)
	{
		i = 0;
		while (newstr[i] && newstr[i] != '\n')
			i++;
		if (newstr[i] == '\n')
			return (1);
	}
	else
		while (str)
		{
			i = 0;
			while (str->line[i] && str->line[i] != '\n')
				i++;
			if (str->line[i] == '\n')
				return (1);
			str = str->next;
		}
	return (0);
}

int		fill_in(t_str **str, char *buffer, int fd)
{
	int		bytes_read;
	t_str	*newline;
	t_str	*elem;

	if ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = 0;
		if (!(newline = (t_str*)malloc(sizeof(*newline)))
			|| !(newline->line = ft_strdup(buffer)))
			return (-2);
		newline->next = NULL;
		if (!*str)
			*str = newline;
		else
		{
			elem = *str;
			while (elem->next)
				elem = elem->next;
			elem->next = newline;
		}
		return (1);
	}
	return ((bytes_read < 0) ? -1 : 0);
}
