/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:16:45 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/21 15:10:12 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	swap_str(int i, char **tabstr)
{
	char	*tmp;

	tmp = tabstr[i + 1];
	tabstr[i + 1] = tabstr[i];
	tabstr[i] = tmp;
}

char	**sort_tabstr(char **tabstr)
{
	int		i;

	i = 0;
	while (tabstr[i + 1] != NULL)
	{
		if (ft_strcmp(tabstr[i], tabstr[i + 1]) > 0)
		{
			swap_str(i, tabstr);
			i = -1;
		}
		i++;
	}
	return (tabstr);
}

int		is_equal(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

char	*put_quote(char *str)
{
	int		j;
	char	*newline;

	j = 0;
	newline = malloc(sizeof(newline) * (ft_strlen(str) + 3));
	while (str[j] && str[j] != '=')
	{
		newline[j] = str[j];
		j++;
	}
	newline[j++] = '=';
	newline[j++] = '\"';
	while (str[j - 1] != '\0')
	{
		newline[j] = str[j - 1];
		j++;
	}
	newline[j] = '\"';
	newline[++j] = '\0';
	return (newline);
}

char	**put_between_quote(char **export_env)
{
	int		i;
	char	*newline;

	i = 0;
	while (export_env[i] != NULL)
	{
		if (is_equal(export_env[i]) == TRUE)
		{
			newline = put_quote(export_env[i]);
			free(export_env[i]);
			export_env[i] = newline;
		}
		i++;
	}
	return (export_env);
}

int		print_exported_var(char **alloc_env)
{
	int		i;

	i = 0;
	alloc_env = sort_tabstr(alloc_env);
	alloc_env = put_between_quote(alloc_env);
	while (alloc_env[i] != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(alloc_env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (1);
}

int		is_bad_char(char c)
{
	if (c == '\"' || c == '\\' || c == '\'')
		return (TRUE);
	return (FALSE);
}

char	**ft_add_tabstrdup(char **tabstr, char *var)
{
	int		j;
	char	**new_tabstr;

	j = 0;
	while (tabstr[j] != NULL)
		j++;
	if (!(new_tabstr = malloc(sizeof(tabstr) * (j + 2))))
		return (NULL);
	j = 0;
	while (tabstr[j])
	{
		new_tabstr[j] = ft_strdup(tabstr[j]);
		j++;
	}
	new_tabstr[j] = ft_strdup(var);
	new_tabstr[++j] = NULL;
	return (new_tabstr);
}

void	bad_identifier(char *arg, char *var)
{
	ft_putstr_fd("msh : export : \'", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
	free(var);
	return ;
}

int		is_export(char **env, char *var)
{
	int		i;
	int		j;

	j = 0;
	while (env[j])
	{
		i = 0;
		while (env[j][i] && env[j][i] == var[i] && env[j][i] != '=')
			i++;
		if (env[j][i] == '\0' || env[j][i] == '=')
			return (j);
		j++;
	}
	return (FAILURE);
}

char	***ft_replace_var(char ***env, char *var)
{
	int		j;
	int		i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if (var[i] == '=')
	{
		j = is_export(env[0], var);
		free(env[0][j]);
		env[0][j] = ft_strdup(var);
	}
	return (env);
}

int		get_new_var(char *arg, char **var)
{
	int		i;

	i = 0;
	if (ft_isdigit(arg[i]) || is_char(arg[i], "\"\\\';") || arg[i] == '\0')
	{
		bad_identifier(arg, var[0]);
		return (FAILURE);
	}
	while (arg[i] && is_char(arg[i], "\"\\\';") == FALSE)
	{
		var[0][i] = arg[i];
		i++;
	}
	if (is_char(arg[i], "\"\\\';") == TRUE)
	{
		bad_identifier(arg, var[0]);
		return (FAILURE);
	}
	var[0][i] = '\0';
	return (SUCCESS);
}

int		add_var(char *arg, char ***env)
{
	int		i;
	char	*var;
	char	**save;

	i = 0;
	save = env[0];
	while (arg[i])
		i++;
	var = malloc(sizeof(var) * (i + 1));
	if (get_new_var(arg, &var) == FAILURE)
		return (FAILURE);
	if (is_export(env[0], var) != FAILURE)
	{
		ft_replace_var(env, var);
	}
	else
	{
		env[0] = ft_add_tabstrdup(env[0], var);
		free_tab(save);
	}
	free(var);
	return (SUCCESS);
}

int		ft_export(char **arg_tab, char ***env)
{
	int		i;
	int		ret;
	char	**alloc_env;

	i = 1;
	ret = 2;
	alloc_env = ft_tabstrdup(env[0]);
	if (arg_tab[i] == NULL)
		print_exported_var(alloc_env);
	else
	{
		while (arg_tab[i] != NULL)
		{
			if (add_var(arg_tab[i], env) == FAILURE)
			{
				g_quit = 1;
				ret = 1;
			}
			i++;
		}
	}
	free_tab(alloc_env);
	return (ret);
}
