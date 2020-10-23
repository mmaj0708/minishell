/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 12:07:09 by user42            #+#    #+#             */
/*   Updated: 2020/10/20 11:36:41 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_search(char *word)
{
	int		i;
	int		j;

	i = 0;
	while (g_env[i])
	{
		j = 0;
		while (g_env[i][j] == word[j])
		{
			j++;
			if (g_env[i][j] == '=' && (is_char(word[j], "/") || word[j] == '\0'))
				return (g_env[i] + j + 1);
		}
		i++;
	}
	word[0] = '\0';
	return (word);
}

int		ft_env(char **tab)
{
	int		i;

	i = -1;
	if (tab[1] != 0)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		g_quit = 1;
		return (1);
	}
	while (g_env[++i])
	{
		ft_putstr_fd(g_env[i], 1);
		ft_putchar_fd('\n', 1);
	}
	return (2);
}
