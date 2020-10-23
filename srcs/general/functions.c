/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:11:51 by hcanon            #+#    #+#             */
/*   Updated: 2020/10/01 14:41:29 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		free_tab(char **tab)
{
	int		i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

void		print_prompt(int error)
{
	if (error == 0)
		ft_putstr(BRED);
	else if (error == 1)
	{
		ft_putstr(BBLUE);
		g_quit = 0;
	}
	ft_pwd(NULL, 0);
	ft_putstr(BGREEN);
	ft_putstr_fd(" $> ", 1);
	ft_putstr(WHITE);
}

int			is_char(char c, char *strchar)
{
	int i;

	i = 0;
	while (strchar[i] && strchar[i] != c)
		i++;
	if (strchar[i] && strchar[i] == c)
		return (TRUE);
	return (FALSE);
}

void		print_strs(char **tabstr)
{
	int i;

	i = 0;
	while (tabstr[i])
	{
		ft_putstr_fd(tabstr[i], 1);
		if (tabstr[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	return ;
}

char		**ft_tabstrdup(char **tabstr)
{
	int		j;
	char	**new_tabstr;

	j = 0;
	while (tabstr[j] != NULL)
		j++;
	if (!(new_tabstr = malloc(sizeof(tabstr) * (j + 1))))
		return (NULL);
	j = 0;
	while (tabstr[j])
	{
		new_tabstr[j] = ft_strdup(tabstr[j]);
		j++;
	}
	new_tabstr[j] = NULL;
	return (new_tabstr);
}
