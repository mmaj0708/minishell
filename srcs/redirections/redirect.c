/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/09 17:05:59 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_redirect_sup(char *command)
{
	int		i;

	i = 0;
	while (command[i])
	{
		if (is_char(command[i], ">") == TRUE)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

void	free_redirect_sup(char **arg_tab, int *fd)
{
	int		j;

	free_tab(arg_tab);
	j = -1;
	while (fd && fd[++j])
		close(fd[j]);
	if (fd)
		free(fd);
}

int		threading(char **arg_tab, int *fd, int j)
{
	pid_t	child;

	child = fork();
	if (child == 0)
	{
		if (dup2(fd[j], 1) != FAILURE)
			check_exceptions(arg_tab[0], 1, 0);
		free_redirect_sup(arg_tab, fd);
		return (FAILURE);
	}
	else if (child < 0)
	{
		ft_putstr_fd("msh: failed to create a new thread\n", 2);
		g_quit = 126;
		return (0);
	}
	wait(&child);
	return (1);
}

int		*initialize_fd(char ***arg_tab, int i, int j)
{
	int		*fd;
	char	*tmp;
	int		k;

	while ((*arg_tab)[i])
		i++;
	if (!(fd = (int *)malloc(sizeof(int) * i)))
		return (NULL);
	while (i-- > 1)
	{
		k = 0;
		tmp = ft_strtrim((*arg_tab)[j + 1], " ");
		while (tmp[k] != '\0' && tmp[k] != ' ')
			k++;
		if (tmp[k])
		{
			tmp[k] = '\0';
			while (tmp[++k])
				*arg_tab[0] = ft_charjoin((*arg_tab)[0], tmp[k]);
		}
		if (!(fd[j] = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0666)))
		{
			free(fd);
			free(tmp);
			return (NULL);
		}
		j++;
		free(tmp);
	}
	fd[j] = 0;
	return (fd);
}

int		redirect_sup(char *line)
{
	char	**arg_tab;
	int		j;
	int		*fd;
	int		i;

	j = 0;
	i = 1;
	arg_tab = ft_split(line, '>');
	arg_tab = get_proper_arg(arg_tab);
	if ((fd = initialize_fd(&arg_tab, 0, i - 1)) == NULL)
	{
		free_tab(arg_tab);
		g_quit = 126;
		return (FALSE);
	}
	while (fd[j])
		j++;
	if (fd[0] == 0)
		i = check_exceptions(arg_tab[0], 1, 0);
	else
		i = threading(arg_tab, fd, j - 1);
	free_redirect_sup(arg_tab, fd);
	return (i);
}
