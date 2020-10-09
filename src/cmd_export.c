/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonroy- <mmonroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 11:24:48 by mmonroy-          #+#    #+#             */
/*   Updated: 2020/10/09 12:03:08 by mmonroy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		free_double(char **str)
{
	int i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	return ;
}

static char		*check_str(char *str)
{
	int		i;
	char	*rst;

	i = 0;
	while (str[i])
		if (str[i++] == '=')
			return (rst = ft_strdup(str));
	i = -1;
	rst = ft_calloc(ft_strlen(str) + 2, sizeof(char));
	while (str[++i])
		rst[i] = str[i];
	rst[i] = '=';
	return (rst);
}

static void		new_g_env2(char *str, char **temp, int i, int j)
{
	int sum;

	sum = 0;
	while (g_env[++j])
		if (ft_strncmp(str, g_env[j], ft_find_char(str, '=') + 1) == 0)
		{
			if (!(temp[j] = ft_strdup(str)))
				exit_minishell();
			sum = 1;
			break ;
		}
	if (!g_env[j])
		if (!(temp[i] = ft_strdup(str)))
			exit_minishell();
	while (g_env[i++])
	{
		if (!(temp[i - sum] = ft_strdup(g_env[i - 1])))
			exit_minishell();
	}
	return ;
}

static void		new_g_env(char *str)
{
	char	**temp;
	char	**buff;
	int		i;
	int		j;

	i = -1;
	j = -1;
	temp = (char**)ft_calloc(envp_len(g_env) + 2, sizeof(char*));
	while (g_env[++i] && ft_strncmp("_=", g_env[i], 2) != 0)
		if (!(temp[i] = ft_strdup(g_env[i])))
			exit_minishell();
	str = check_str(str);
	new_g_env2(str, temp, i, j);
	buff = g_env;
	g_env = temp;
	free_double(buff);
}

void			cmd_export(char **arg)
{
	int i;

	i = 0;
	if (!arg[1])
	{
		while (g_env[i])
		{
			write(1, g_env[i], ft_strlen(g_env[i]));
			write(1, "\n", 1);
			if (ft_strncmp("__CF", g_env[i++], 4) == 0)
				return ;
		}
		return ;
	}
	while (arg[++i])
		new_g_env(arg[i]);
	return ;
}
