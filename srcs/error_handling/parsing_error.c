/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 11:32:35 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/19 15:03:34 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_condition(t_quote *q, int *i, char **quoted_line, char *line)
{
	if (line[i[0]] == '\"' && q->singl % 2 == 0 && line[i[0] - 1] != '\\')
	{
		q->doubl++;
		quoted_line[0][i[0]++] = '0';
	}
	else if ((line[i[0]] == '\'' && q->doubl % 2 == 0 && line[i[0] - 1] != '\\')
	|| (line[i[0]] == '\'' && q->doubl % 2 == 0 && q->singl % 2 == 1))
	{
		q->singl++;
		quoted_line[0][i[0]++] = '0';
	}
	else if ((q->singl % 2 == 1 || q->doubl % 2 == 1))
		quoted_line[0][i[0]++] = '1';
	else if (line[i[0]] == '\\' && line[i[0] + 1] != '\0'
	&& (q->singl % 2 == 0 && q->doubl % 2 == 0))
	{
		quoted_line[0][i[0]] = '0';
		quoted_line[0][i[0] + 1] = '1';
		i[0] = i[0] + 2;
	}
	else
		quoted_line[0][i[0]++] = '0';
}

int		count_quote(char *line, char **quoted_line)
{
	int		i;
	t_quote	q;

	i = 0;
	q.singl = 0;
	q.doubl = 0;
	while (line[i])
	{
		quote_condition(&q, &i, quoted_line, line);
	}
	quoted_line[0][i] = '\0';
	if (is_char(line[i - 1], "\\") == TRUE)
		return (FAILURE);
	if (q.singl % 2 == 1 || q.doubl % 2 == 1)
		return (FAILURE);
	return (SUCCESS);
}

int		check_bad_char(char *line, int i)
{
	int save;

	save = i;
	i++;
	while (line[i] && line[i] == ' ')
		i++;
	if (is_char(line[i], "|;") == TRUE)
		return (FAILURE);
	while (save > 0 && line[--save] == ' ')
		save = save + 0;
	if (save == 0 || is_char(line[i], "|;") == TRUE)
		return (FAILURE);
	return (SUCCESS);
}

int		check_semicolon(char *line, char *quoted_line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ';' && quoted_line[i] == '0'
		&& check_bad_char(line, i) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int		check_bad_redirection(char *line, int i)
{
	i++;
	if (line[i] == '>' && line[i - 1] == '>')
		i++;
	while (line[i] && line[i] == ' ')
		i++;
	if (line[i] == '\0' || is_char(line[i], "|;<>") == TRUE)
		return (FAILURE);
	return (SUCCESS);
}

int		check_redirection(char *line, char *quoted_line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (is_char(line[i], "<>") && quoted_line[i] == '0'
		&& check_bad_redirection(line, i) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int		check_pipe(char *line, char *quoted_line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|' && quoted_line[i] == '0'
		&& check_bad_char(line, i) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int		parsing_error(char *line)
{
	char	*quoted_line;

	quoted_line = malloc(sizeof(quoted_line) * (ft_strlen(line) + 1));
	if (count_quote(line, &quoted_line) == FAILURE)
	{
		free(quoted_line);
		return (FAILURE);
	}
	if (check_semicolon(line, quoted_line) == FAILURE)
	{
		free(quoted_line);
		return (FAILURE);
	}
	if (check_redirection(line, quoted_line) == FAILURE)
	{
		free(quoted_line);
		return (FAILURE);
	}
	if (check_pipe(line, quoted_line) == FAILURE)
	{
		free(quoted_line);
		return (FAILURE);
	}
	free(quoted_line);
	return (SUCCESS);
}
/*
**si le dernier char est '\' renvoyer FAILURE
*/
