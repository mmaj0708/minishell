/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 14:46:52 by user42            #+#    #+#             */
/*   Updated: 2020/10/15 14:16:43 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			compute_line(char **arg_tab)
{
	if (strcmp(arg_tab[0], "echo") == 0)
		return (ft_echo(arg_tab));
	else if (strcmp(arg_tab[0], "pwd") == 0)
		return (ft_pwd(arg_tab[1], 1));
	else if (strcmp(arg_tab[0], "env") == 0)
		return (ft_env(arg_tab));
	else if (strcmp(arg_tab[0], "cd") == 0)
		return (ft_cd(arg_tab));
	else if (strcmp(arg_tab[0], "export") == 0)
		return (ft_export(arg_tab, &g_env));
	else if (strcmp(arg_tab[0], "unset") == 0)
		return (ft_unset(arg_tab, &g_env));
	else if (strcmp(arg_tab[0], "exit") == 0)
		return (ft_exit(arg_tab));
	return (0);
}

int			check_exceptions(char *line, int type, int exit)
{
	char	**command_tab;
	int		i;

	i = -1;
	command_tab = ft_split(line, ';');
	while (command_tab[++i])
	{
		if (is_double_redirect(command_tab[i]) == TRUE)
		{
			if ((exit = double_redirect(command_tab[i])) == FAILURE)
				break ;
		}		
		if (type == 0 && is_redirect_sup(command_tab[i]) == TRUE)
		{
			if ((exit = redirect_sup(command_tab[i])) == FAILURE)
				break ;
		}
		else if (is_redirect_inf(command_tab[i]) == TRUE)
		{
			if ((exit = redirect_inf(command_tab[i])) == FAILURE)
				break ;
		}
		else if (is_char_no_quot(command_tab[i], '|') == TRUE)
		{
			if ((exit = pipeline(command_tab[i])) == FAILURE)
				break ;
		}
		else
		{
			if ((exit = find_job(command_tab[i])) == FAILURE)
				break ;
		}
	}
	free_tab(command_tab);
	return (exit);
}

void		main_loop(void)
{
	char	*line;
	int		exit;

	exit = 1;
	while (1)
	{
		signal(SIGINT, ctrlc_handler);
		print_prompt(exit);
		get_next_line(1, &line);
		if (parsing_error(line) != FAILURE)
		{
			if ((exit = check_exceptions(line, 0, 0)) == FAILURE)
				break ;
		}
		else
		{
			ft_putstr_fd("msh: error: incorrect syntax\n", 2);
			exit = 0;
			g_quit = 1;
		}
		free(line);
	}
	free_tab(g_env);
	free(line);
	get_next_line(-1, &line);
}

int			main(int ac, char **av, char **envp)
{
	(void)av;
	if (ac == 1)
	{
		g_quit = 0;
		g_pipe = 0;
		signal(SIGQUIT, ctrlc_handler);
		g_env = ft_tabstrdup(envp);
		main_loop();
		return (0);
	}
	ft_putstr_fd("too many arguments\n", 2);
	return (-1);
}
