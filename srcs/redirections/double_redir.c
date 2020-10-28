/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/09 17:05:59 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_double_redirect(char *command)
{
	int		i;
	t_quote	quote;

	quote.singl = 0;
	quote.doubl = 0;
	i = 0;
	while (command[i])
	{
		if (command[i] == '\'')
			quote.singl++;
		if (command[i] == '\"')
			quote.doubl++;
		if (command[i] == '>' && command[i + 1] == '>' &&
		is_even(quote.singl) == TRUE && is_even(quote.doubl) == TRUE)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		redir_double_count(char *line)
{
	int		i;
	int		j;

	j = 0;
	i = -1;
	while (line[++i])
		if (is_char(line[i], ">") == TRUE && is_char(line[i + 1], ">") == TRUE)
			j++;
	return (j);
}

void	free_double_redirect(char **arg_tab, int *fd)
{
	int		j;

	free_tab(arg_tab);
	j = -1;
	while (fd && fd[++j])
		close(fd[j]);
	if (fd)
		free(fd);
}

int		threadingd(char **arg_tab, int *fd, int j)
{
	pid_t	child;

	child = fork();
	if (child == 0)
	{
		if (dup2(fd[j], 1) != FAILURE)
			check_exceptions(arg_tab[0], 1, 0);
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

int		*initialize_fdd(char ***arg_tab, int i, int j)
{
	int		*fd;
	char	*tmp;

	while ((*arg_tab)[i])
		i++;
	if (!(fd = (int *)malloc(sizeof(int) * i)))
		return (NULL);
	while (i-- > 1)
	{
		tmp = (*arg_tab)[j + 1];
		ft_putstr_fd(tmp, 1);
		if (!(fd[j] = open(tmp, O_CREAT | O_RDWR | O_APPEND, 0777)))
		{
			free(fd);
			return (NULL);
		}
		j++;
	}
	fd[j] = 0;
	return (fd);
}

char	*word_double(char *line, int *i)
{
	char	*str;
	int		j;

	j = -1;
	if (!(str = malloc(sizeof(char) * ft_strlen(line + *i) + 1)))
		return (NULL);
	(*i)++;
	(*i)++;
	while (line[*i] && line[*i] == ' ')
		(*i)++;
	while (line[*i] && line[*i] != ' ')
		str[++j] = line[(*i)++];
	str[++j] = 0;
	return (str);
}

char	**copy_stringd(char *str)
{
	char	*line;
	char	**arg_tab;
	int		i;
	int		j;
	t_quote	quote;

	i = -1;
	j = 1;
	quote.doubl = 0;
	quote.singl = 0;
	line = ft_strdup(" ");
	if (!(arg_tab = malloc(sizeof(char *) * (redir_double_count(line) + 1))))
		return (NULL);
	while (str[++i])
		line = ft_charjoin(line, str[i]);
	while (line[++i])
	{
		if (line[i] == '\'')
			quote.singl++;
		if (line[i] == '\"')
			quote.doubl++;
		if (line[i] == '>' && line[i + 1] == '>' &&
		is_even(quote.singl) == TRUE && is_even(quote.doubl) == TRUE)
			arg_tab[j++] = word_double(line, &i);
		arg_tab[0] = ft_charjoin(arg_tab[0], line[i]);
		if (line[i] == 0)
			break ;
	}
	free(line);
	return (arg_tab);
}

int		double_redirect(char *str)
{
	char	**arg_tab;
	int		j;
	int		*fd;
	int		i;

	j = 0;
	i = 1;
	arg_tab = copy_stringd(str);
	arg_tab = get_proper_arg(arg_tab);
	if ((fd = initialize_fdd(&arg_tab, 0, i - 1)) == NULL)
	{
		free_tab(arg_tab);
		g_quit = 126;
		return (FALSE);
	}
	while (fd[j])
		j++;
	i = threadingd(arg_tab, fd, j - 1);
	free_double_redirect(arg_tab, fd);
	return (i);
}
