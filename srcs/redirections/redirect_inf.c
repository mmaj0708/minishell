/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_inf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/09 17:05:59 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_redirect_inf(char *command)
{
	int		i;

	i = 0;
	while (command[i])
	{
		if (is_char(command[i], "<") == TRUE)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		redirect_inf(char *line)
{
	(void)line;
	return (0);
}