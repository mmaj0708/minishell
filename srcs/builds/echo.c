/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <hcanon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 12:07:09 by user42            #+#    #+#             */
/*   Updated: 2020/09/10 11:28:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_echo(char **tab)
{
	int		option;

	option = 0;
	if (tab[1] != 0)
	{
		while (strcmp(tab[option + 1], "-n") == 0)
			option++;
		print_strs(tab + option + 1);
	}
	if (option == 0)
		ft_putchar_fd('\n', 1);
	return (2);
}
