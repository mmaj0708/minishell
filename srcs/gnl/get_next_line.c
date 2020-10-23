/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcanon <hcanon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 15:19:29 by hcanon            #+#    #+#             */
/*   Updated: 2020/09/04 14:57:06 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		parsing(t_str **str, int fd)
{
	char	*buffer;
	int		bytes_read;

	if (!(buffer = (char *)malloc(sizeof(*buffer) * (BUFFER_SIZE + 1))))
		return (-1);
	while ((bytes_read = fill_in(str, buffer, fd)) > 0)
		if (check_lf(NULL, buffer))
			break ;
	free(buffer);
	if (bytes_read < 0)
		return (-2);
	return (1);
}

int		alloc_mem(t_str **str, char **line)
{
	t_str	*first;
	int		i;
	int		j;
	char	*buffer;

	first = *str;
	j = 0;
	while (*str)
	{
		i = 0;
		while ((*str)->line[i] && (*str)->line[i] != '\n' && ++j)
			i++;
		if ((*str)->line[i] == '\n')
			break ;
		*str = (*str)->next;
	}
	*str = first;
	if (!(buffer = (char *)malloc(sizeof(*buffer) * (j + 1))))
		return (0);
	*line = buffer;
	(*line)[j] = 0;
	return (1);
}

int		str_to_line(t_str **str, char **line)
{
	int		idx[2];
	int		remaining;
	t_str	*next;

	idx[1] = 0;
	remaining = 0;
	while (*str)
	{
		idx[0] = 0;
		while ((*str)->line[idx[0]] && (*str)->line[idx[0]] != '\n')
			(*line)[idx[1]++] = (*str)->line[idx[0]++];
		if ((*str)->line[idx[0]++] == '\n' && (remaining = 1))
		{
			idx[1] = 0;
			while ((*str)->line[idx[0]])
				(*str)->line[idx[1]++] = (*str)->line[idx[0]++];
			(*str)->line[idx[1]] = 0;
			break ;
		}
		next = (*str)->next;
		free((*str)->line);
		free(*str);
		*str = next;
	}
	return (remaining);
}

int		ft_fclean(t_fd **list, int fd, char *buf)
{
	t_fd	*first;
	t_fd	*lt[2];

	first = (list) ? *list : NULL;
	lt[0] = NULL;
	while (list && *list)
	{
		lt[1] = (*list)->next;
		if (fd < 0 || (*list)->fd == fd)
		{
			if (first == (*list))
				first = lt[1];
			lst_clear(&(*list)->str);
			free((*list));
			if (lt[0])
				lt[0]->next = lt[1];
		}
		lt[0] = (*list);
		(*list) = lt[1];
	}
	if (list)
		*list = first;
	if (buf)
		free(buf);
	return (0);
}

int		get_next_line(int fd, char **line)
{
	static t_fd	*list = NULL;
	t_fd		*current_fd;
	int			new;
	char		*buffer;
	int			result;

	if (!(current_fd = find_fd(&list, fd, &new)))
		return (ft_fclean(&list, -1, NULL) | -1);
	if ((buffer = NULL) || (!new && current_fd->str))
		new = !check_lf(current_fd->str, NULL);
	if (new && (result = parsing(&current_fd->str, fd)) < 0)
		return (ft_fclean(&list, (result == -1) ? -1 : fd, NULL) | -1);
	if (!(alloc_mem(&current_fd->str, line)))
		return (ft_fclean(&list, -1, NULL) | -1);
	if (!(new = str_to_line(&current_fd->str, line)))
	{
		if (!(buffer = (char*)malloc(sizeof(*buffer) * (BUFFER_SIZE + 1))))
			return (ft_fclean(&list, -1, NULL));
		result = fill_in(&current_fd->str, buffer, fd);
		if (ft_fclean(NULL, -1, buffer) || result < 0)
			return (ft_fclean(&list, fd, NULL) | -1);
	}
	if (result > 0 || new)
		return (1);
	return (ft_fclean(&list, fd, NULL));
}
