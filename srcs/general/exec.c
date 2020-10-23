/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 14:46:52 by user42            #+#    #+#             */
/*   Updated: 2020/10/19 16:00:00 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exec_program(char *path, char **args)
{
	pid_t	child;

	child = fork();
	signal(SIGINT, cancel_handler);
	if (child == 0)
		execve(path, args, g_env);
	else if (child < 0)
	{
		free(path);
		ft_putstr_fd("msh: failed to create a new thread\n", 2);
		g_quit = 126;
		signal(SIGINT, ctrlc_handler);
		return (0);
	}
	wait(&child);
	if (path)
		free(path);
	signal(SIGINT, ctrlc_handler);
	return (1);
}

int		is_executable(char *bin_path, struct stat f, char **arg_tab)
{
	if (f.st_mode & S_IFREG)
	{
		if (f.st_mode & S_IXUSR)
			return (exec_program(bin_path, arg_tab));
		else
		{
			ft_putstr_fd("msh: permission denied: ", 2);
			ft_putstr_fd(bin_path, 2);
			ft_putchar_fd('\n', 2);
			g_quit = 126;
		}
		free(bin_path);
		return (1);
	}
	free(bin_path);
	return (0);
}

int		check_bins(char **arg_tab)
{
	int				i;
	char			*bin_path;
	char			**path;
	struct stat		f;

	path = ft_split(ft_search("PATH"), ':');
	i = -1;
	while (path && path[++i])
	{
		if (ft_strcmp(arg_tab[0], path[i]) == 0)
			bin_path = ft_strdup(arg_tab[0]);
		else
			bin_path = ft_searchpath(path[i], arg_tab[0]);
		if (lstat(bin_path, &f) == -1)
			free(bin_path);
		else
		{
			free_tab(path);
			return (is_executable(bin_path, f, arg_tab));
		}
	}
	free_tab(path);
	return (0);
}

int		is_correct_file(char **arg_tab)
{
	struct stat	f;

	if (lstat(arg_tab[0], &f) != -1)
	{
		if (f.st_mode & S_IFDIR)
			return (ft_cd(arg_tab) - 1);
		else if (f.st_mode & S_IXUSR)
			return (exec_program(ft_strdup(arg_tab[0]), arg_tab));
	}
	ft_putstr_fd("msh: command not found: ", 2);
	ft_putstr_fd(arg_tab[0], 2);
	ft_putchar_fd('\n', 2);
	g_quit = 127;
	return (0);
}

int		find_job(char *line)
{
	int			is_builtin;
	char		**arg_tab;

	arg_tab = ft_split(line, ' ');
	arg_tab = get_proper_arg(arg_tab);
	if ((is_builtin = compute_line(arg_tab)) > 0 || check_bins(arg_tab))
	{
		free_tab(arg_tab);
		if (is_builtin > 0)
			return (is_builtin - 1);
		return (1);
	}
	if (is_builtin < 0)
	{
		free_tab(arg_tab);
		return (-1);
	}
	is_builtin = is_correct_file(arg_tab);
	free_tab(arg_tab);
	return (is_builtin);
}
