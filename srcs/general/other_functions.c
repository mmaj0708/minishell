/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:11:51 by hcanon            #+#    #+#             */
/*   Updated: 2020/10/21 15:06:30 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_even(int n)
{
	if (n % 2 == 0)
		return (1);
	return (0);
}

char		*ft_searchpath(char *str1, char *str2)
{
	char	*tmp;

	if (!str2 || !str1)
		return (NULL);
	if (str1[ft_strlen(str1) - 1] != '/')
	{
		if (str2[0] == '/')
			return (ft_strjoin(str1, str2));
		else
		{
			tmp = ft_strjoin(str1, "/");
			str1 = ft_strjoin(tmp, str2);
			free(tmp);
			return (str1);
		}
	}
	else
	{
		if (str2[0] == '/')
			return (ft_strjoin(str1, str2 + 1));
		else
			return (ft_strjoin(str1, str2));
	}
}

int			is_replace(char *arg)
{
	int		i;

	i = 0;
	while (arg[i])
	{
		if (is_char(arg[i], "\"\'\\$~"))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

char		*ft_charjoin(char *str, char c)
{
	int		i;
	char	*new_str;

	i = -1;
	if (c == '\0')
		return (str);
	if (!str)
	{
		if (!(new_str = malloc(2 * sizeof(c))))
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	if (!(new_str = malloc(sizeof(c) * (ft_strlen(str) + 2))))
		return (NULL);
	while (str[++i])
		new_str[i] = str[i];
	new_str[i++] = c;
	new_str[i] = '\0';
	free(str);
	return (new_str);
}
