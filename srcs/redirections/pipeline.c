/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/22 15:12:47 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		is_char_no_quot(char *command, char c)
{
	int		i;
	char	*quoted_line;

	i = 0;
	quoted_line = malloc(sizeof(char) * (ft_strlen(command) + 1));
	count_quote(command, &quoted_line);
	while (command[i])
	{
		if (is_char(command[i], &c) == TRUE && quoted_line[i] == '0')
		{
			free(quoted_line);
			return (TRUE);
		}
		i++;
	}
	free(quoted_line);
	return (FALSE);
}

int		count_pipe(char *command)
{
	int		i;
	int		count;
	char	*quoted_line;

	i = 0;
	count = 0;
	quoted_line = malloc(sizeof(char) * (ft_strlen(command) + 1));
	count_quote(command, &quoted_line);
	while (command[i])
	{
		if (command[i] == '|' && quoted_line[i] == '0')
			count++;
		i++;
	}
	free(quoted_line);
	return (count);
}

int		close_fd(int *pipefd, int nb_fd)
{
	int		i;

	i = 0;
	while (i < nb_fd)
	{
		close(pipefd[i]);
		i++;
	}
	return (SUCCESS);
}

int		create_pid_tab(t_pipe *p)
{
	int	i;

	i = 0;
	p->child = malloc(sizeof(pid_t) * p->nb_job);
	while (i < p->nb_job)
	{
		p->child[i] = -1;
		i++;
	}
	return (SUCCESS);
}

int		create_pipefd_tab(t_pipe *p)
{
	int i;

	i = 0;
	p->pipefd = malloc(sizeof(int) * (p->nb_fd));
	while (i < p->nb_fd)
	{
		pipe(&p->pipefd[i]);
		i = i + 2;
	}
	return (SUCCESS);
}

void	connect_pipe(char **job_tab, t_pipe p, int i)
{
	dup2(p.pipefd[i + i - 2], 0);
	dup2(p.pipefd[i + i + 1], 1);
	close_fd(p.pipefd, p.nb_pipe * 2);
	find_job(job_tab[i]);
	exit(g_quit);
}

int		exec_job(char **job_tab, int i, t_pipe p)
{
	if ((p.child[i] = fork()) < 0)
		exit(0);
	if (p.child[0] == 0)
	{
		dup2(p.pipefd[1], 1);
		close_fd(p.pipefd, p.nb_pipe * 2);
		find_job(job_tab[0]);
		wait(NULL);
		exit(g_quit);
	}
	if (p.child[p.nb_job - 1] == 0)
	{
		dup2(p.pipefd[p.nb_fd - 2], 0);
		close_fd(p.pipefd, p.nb_pipe * 2);
		find_job(job_tab[p.nb_job - 1]);
		exit(g_quit);
	}
	if (p.child[i] == 0)
		connect_pipe(job_tab, p, i);
	return (0);
}

int		init_pipe(char *command, t_pipe *p, char ***job_tab)
{
	p->nb_pipe = count_pipe(command);
	job_tab[0] = ft_split(command, '|');
	p->nb_job = p->nb_pipe + 1;
	p->nb_fd = p->nb_pipe * 2;
	create_pid_tab(p);
	create_pipefd_tab(p);
	return (SUCCESS);
}

int		pipeline(char *command)
{
	int		i;
	t_pipe	p;
	char	**job_tab;
	int 	stat;

	i = 0;
	g_pipe = 1;
	init_pipe(command, &p, &job_tab);
	while (i < p.nb_job)
	{
		exec_job(job_tab, i, p);
		i++;
	}
	close_fd(p.pipefd, p.nb_pipe * 2);
	i = 0;
	while (i < p.nb_job)
	{
		waitpid(p.child[i], &stat, 0);
		if (WIFEXITED(stat))
			g_quit = WEXITSTATUS(stat);
		i++;
	}
	free(p.child);
	free(p.pipefd);
	free_tab(job_tab);
	return (g_quit);
}
