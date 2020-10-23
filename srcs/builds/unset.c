/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 09:32:09 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/20 15:14:30 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_remove_tabstrdup(char **tabstr, int index)
{
	int		j;
	char	**new_tabstr;

	j = 0;
	while (tabstr[j] != NULL)
		j++;
	if (!(new_tabstr = malloc(sizeof(tabstr) * (j))))
		return (NULL);
	j = 0;
	while (j < index)
	{
		new_tabstr[j] = ft_strdup(tabstr[j]);
		j++;
	}
	index++;
	while (tabstr[index] != NULL)
	{
		new_tabstr[j] = ft_strdup(tabstr[index]);
		index++;
		j++;
	}
	new_tabstr[j] = NULL;
	return (new_tabstr);
}

int		is_good_var(char *var)
{
	int		i;

	i = 0;
	while (var[i] == ' ')
		i++;
	if (is_char(var[i], "=1234567890\\\"\' ") == TRUE || var[i] == '\0')
		return (FALSE);
	i++;
	while (var[i])
	{
		if (is_char(var[i], " \\\'\"") == TRUE)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int		ft_unset(char **arg_tab, char ***env)
{
	int		j;
	int		i;
	int		ret;
	char	**save;

	i = 1;
	ret = 2;
	while (arg_tab[i])
	{
		save = env[0];
		if (is_good_var(arg_tab[i]) == TRUE &&
			(j = is_export(env[0], arg_tab[i])) != FAILURE)
		{
			env[0] = ft_remove_tabstrdup(env[0], j);
			free_tab(save);
		}
		if (is_good_var(arg_tab[i]) == FALSE)
		{
			bad_identifier(arg_tab[i], NULL);
			g_quit = 1;
			ret = 1;
		}
		i++;
	}
	return (ret);
}
