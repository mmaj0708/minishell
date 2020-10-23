/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcanon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 12:46:55 by hcanon            #+#    #+#             */
/*   Updated: 2020/09/07 11:16:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*newline(const char *str, int *i, char c)
{
	char	*newline;
	int		j;
	int		doubles;
	int		singles;

	j = 0;
	doubles = 0;
	singles = 0;
	if (!(newline = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1))))
		return (0);
	while ((doubles % 2 == 1 || singles % 2 == 1 || str[*i] != c ||
			(*i != 0 && str[*i - 1] == '\\')) && str[*i])
	{
		if (str[*i] == '\"')
			doubles++;
		if (str[*i] == '\'')
			singles++;
		newline[j++] = str[(*i)++];
	}
	newline[j] = 0;
	return (newline);
}

char			**ft_split(char const *s, char c)
{
	char	**strs;
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (!s)
		return (NULL);
	if (!(strs = (char**)malloc(sizeof(char *) * (ft_strlen((char *)s) + 1))))
		return (NULL);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == 0)
		{
			strs[j] = 0;
			return (strs);
		}
		if (!(strs[j] = newline((char*)s, &i, c)))
			return (0);
		j++;
	}
	strs[j] = 0;
	return (strs);
}
