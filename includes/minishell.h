/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:11:51 by hcanon            #+#    #+#             */
/*   Updated: 2020/10/23 10:05:02 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define WHITE	"\033[0m"
# define RED	"\033[0;31m"
# define BRED	"\033[1;31m"
# define GREEN	"\033[0;32m"
# define BGREEN	"\033[1;32m"
# define BLUE	"\033[0;34m"
# define BBLUE	"\033[1;34m"
# define CYAN	"\033[0;36m"
# define BCYAN	"\033[1;36m"
# define FAILURE -1
# define TRUE 1
# define SUCCESS 0
# define FALSE 0
# define S_IFDIR 0040000
# define S_IFREG 0100000

# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../libft/libft.h"
# include "get_next_line.h"

typedef	struct	s_quote
{
	int		singl;
	int		doubl;
}				t_quote;

typedef	struct	s_pipe
{
	int		nb_fd;
	int		nb_job;
	int		nb_pipe;
	int		*pipefd;
	pid_t	*child;
}				t_pipe;


// built-in functions
int				ft_echo(char **arg_tab);
int				ft_pwd(char *line, int type);
int				ft_cd(char **arg_tab);
int				ft_env(char **arg_tab);
int				ft_export(char **arg_tab, char ***env);
int				ft_unset(char **arg_tab, char ***env);
int				ft_exit(char **arg_tab);
// pipes functions
int				is_pipe(char *command);
int				pipeline(char *command, char **command_tab);

// other
void			print_strs(char **tabstr);
void			free_tab(char **tab);
void			print_env_var(char *line, int *i);
void			ctrlc_handler(int signum);
void			cancel_handler(int signum);
void			print_prompt(int error);
char			**get_proper_arg(char **arg_tab);
char			*ft_search(char *word);
char			**ft_tabstrdup(char **tabstr);
int				compute_line(char **arg_tab);
int				find_job(char *line);
int				parsing_error(char *line);
int				is_char(char c, char *strchar);
int				is_export(char **env, char *var);
int				ft_echo_print(char *line);
int				is_even(int n);
int				is_redirect_sup(char *command);
int				redirect_sup(char *command);
int				is_redirect_inf(char *command);
int				redirect_inf(char *command);
int				is_redir(char *command);
int				redirection(char *command);
int				check_exceptions(char *line, int type, int exit);
char			*ft_searchpath(char *str1, char *str2);
int				is_replace(char *arg);
char			*ft_charjoin(char *str, char c);
int				count_quote(char *line, char **quoted_line);
void			bad_identifier(char *arg, char *var);

char			**g_env;
int				g_quit;

#endif
