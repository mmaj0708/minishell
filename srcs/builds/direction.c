/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <hcanon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 12:07:09 by user42            #+#    #+#             */
/*   Updated: 2020/09/10 11:28:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_pwd(char *line, int type)
{
	char	**tab;
	char	cwd[1024];
	int		i;

	i = 0;
	(void)line;
	getcwd(cwd, sizeof(cwd));
	(type == 1) ? ft_putstr_fd(cwd, 1) : 0;
	if (type == 0)
	{
		tab = ft_split(cwd, '/');
		while (tab[i] != 0)
			i++;
		(i == 0) ? ft_putstr_fd("/", 1) : 0;
		(i > 0) ? ft_putstr_fd(tab[i - 1], 1) : 0;
		free_tab(tab);
		return (2);
	}
	ft_putchar_fd('\n', 1);
	return (2);
}

void	print_cd_error(char **tab)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(tab[1], 2);
	ft_putchar_fd('\n', 2);
}

int		ft_cd(char **tab)
{
	int		i;
	int		exit;

	exit = 2;
	i = 0;
	if (strcmp(tab[0], "cd") == 0)
		i++;
	if (tab[i] == 0)
		return (chdir(ft_search("HOME")) + 2);
	else if (tab[i + 1] != 0)
	{
		ft_putstr_fd("cd: string not in pwd: ", 2);
		ft_putstr_fd(tab[1], 2);
		ft_putchar_fd('\n', 2);
		return (g_quit = 1);
	}
	while (tab[i][ft_strlen(tab[i]) - 1] == '/')
		tab[i][ft_strlen(tab[i]) - 1] = '\0';
	if (chdir(tab[i]) == -1)
	{
		print_cd_error(tab);
		exit = 1;
		g_quit = 1;
	}
	return (exit);
}
