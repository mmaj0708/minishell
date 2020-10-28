/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_proper_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 16:27:37 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/21 15:47:02 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			special_var(char **proper_arg)
{
	char	*tmp;
	char	*tmp2;

	tmp2 = ft_itoa(g_quit);
	tmp = ft_strjoin(*proper_arg, tmp2);
	free(tmp2);
	free(*proper_arg);
	*proper_arg = tmp;
	return (SUCCESS);
}

int			get_ve_value(char **proper_arg, char *arg, int i)
{
	char	*str_name;
	char	*tmp;
	int		j;

	j = 0;
	if (arg[i] == '?') // faut return direct ?
		return (special_var(proper_arg));
	if (ft_isalnum(arg[i]) == FALSE)
	{
		proper_arg[0] = ft_charjoin(proper_arg[0], '$');
		return (SUCCESS);
	}
	if (!(str_name = malloc(sizeof(char) * ft_strlen(arg) + 1)))
		return (FAILURE);
	while (arg[i] && is_char(arg[i], "\\\'\"$ ") == FALSE)
		str_name[j++] = arg[i++];
	tmp = ft_strjoin(*proper_arg, ft_search(str_name));
	free(str_name);
	free(*proper_arg);
	*proper_arg = tmp;
	return (SUCCESS);
}

int			pass_ve(char *arg, int i)
{
	i++;
	if (is_char(arg[i], "0123456789") == TRUE)
		return (++i);
	while (arg[i] && is_char(arg[i], "\\\'\"$/ ") == FALSE)
		i++;
	return (i);
}

void		replace_arg_cond(t_quote *q, int *i, char *arg, char **proper_arg)
{
	// if (arg[i[0]] == '\\' && (is_even(q->singl) == 0 || is_even(q->doubl) == 0))
	// {
	// 	proper_arg[0] = ft_charjoin(proper_arg[0], arg[i[0]]);
	// 	i[0]++;
	// }
	if (arg[i[0]] == '\\' && (is_even(q->singl) == 0 ||
			(is_even(q->doubl) == 0 && arg[i[0] + 1] != '\"')))
		proper_arg[0] = ft_charjoin(proper_arg[0], arg[i[0]]);
	else if (arg[i[0]] == '\\')
		proper_arg[0] = ft_charjoin(proper_arg[0], arg[++i[0]]);
	else if (arg[i[0]] == '\'' && is_even(q->doubl))
		q->singl++;
	else if (arg[i[0]] == '\"' && is_even(q->singl))
		q->doubl++;
	else if (arg[i[0]] == '$' && is_even(q->singl))
	{
		get_ve_value(proper_arg, arg, i[0] + 1);
		i[0] = pass_ve(arg, i[0]) - 1;
	}
	else
		proper_arg[0] = ft_charjoin(proper_arg[0], arg[i[0]]);
}

char		*replace_arg(char *arg)
{
	int		i;
	t_quote	q;
	char	*proper_arg;
	char	*save;

	i = -1;
	q.doubl = 0;
	q.singl = 0;
	if (!(proper_arg = malloc(sizeof(char) * 1)))
		return (NULL);
	proper_arg[0] = '\0';
	if (arg[0] == '~' && (arg[1] == '\0' || arg[1] == '/'))
	{
		save = proper_arg;
		proper_arg = ft_strjoin(proper_arg, ft_search("HOME"));
		free(save);
		i++;
	}
	while (arg[++i])
		replace_arg_cond(&q, &i, arg, &proper_arg);
	return (proper_arg);
}

char		**get_proper_arg(char **arg_tab)
{
	int		j;
	char	*proper_arg;

	j = 0;
	while (arg_tab[j])
	{
		if (is_replace(arg_tab[j]) == TRUE)
		{
			proper_arg = replace_arg(arg_tab[j]);
			free(arg_tab[j]);
			arg_tab[j] = proper_arg;
		}
		j++;
	}
	return (arg_tab);
}

/*
** l'arg peut etre sans "" ou ''
** s'il y a un seul " ou ' il se termine forcément
**
**
** echo $'' --> rien
** echo "$''" --> $''
** peu de gens ont géré ça
*/
