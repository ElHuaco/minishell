/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonroy- <mmonroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 11:30:19 by aleon-ca          #+#    #+#             */
/*   Updated: 2020/10/26 10:24:45 by aleon-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_start_exec_path(char **strdir)
{
	char	*temp[2];

	if (!*strdir || (ft_strlen(*strdir) <= 1))
		return (0);
	if (**strdir == '/')
		return (1);
	if (is_absolute_path(strdir))
		return (1);
	if (*(*strdir) == '.')
	{
		if (((*(*strdir + 1) == '.') && (*(*strdir + 2) == '/'))
			|| (*(*strdir + 1) == '/'))
			return (1);
		return (0);
	}
	else if ((*(*strdir) == '~') && (*(*strdir + 1) == '/'))
	{
		temp[0] = env_selector("HOME");
		temp[1] = ft_strjoin(temp[0], *strdir + 1);
		free(temp[0]);
		free(*strdir);
		*strdir = temp[1];
		return (1);
	}
	return (0);
}

int		is_built_in(char **arr)
{
	if (!arr[0])
		return (0);
	else if (!(ft_strcmp(arr[0], "echo")))
		return (1);
	else if (!(ft_strcmp(arr[0], "cd")))
		return (1);
	else if (!(ft_strcmp(arr[0], "pwd")))
		return (1);
	else if (!(ft_strcmp(arr[0], "export")))
		return (1);
	else if (!(ft_strcmp(arr[0], "unset")))
		return (1);
	else if (!(ft_strcmp(arr[0], "env")))
		return (1);
	else if (!(ft_strcmp(arr[0], "exit")))
		return (1);
	else
		return (0);
}

void	choose__exec(char **arr, t_command_table *tab)
{
	if (!(ft_strcmp(arr[0], "echo")))
		cmd_echo(arr);
	else if (!(ft_strcmp(arr[0], "cd")) && !((tab->simple_commands_num != 1)
		&& (ft_arrlen(arr) == 1)))
		cmd_cd(arr);
	else if (!(ft_strcmp(arr[0], "pwd")))
		cmd_pwd(arr);
	else if (!(ft_strcmp(arr[0], "export")) && (tab->simple_commands_num == 1))
		cmd_export(arr);
	else if (!(ft_strcmp(arr[0], "unset")) && (tab->simple_commands_num == 1))
		cmd_unset(arr);
	else if (!(ft_strcmp(arr[0], "env")))
		cmd_env(arr);
	else if (!(ft_strcmp(arr[0], "exit")))
		cmd_exit();
}

int		is_cmd_cd(char **arr, int simple_commands_num)
{
	if (!(ft_strcmp(arr[0], "cd")) && !((ft_arrlen(arr) == 1)
		&& (simple_commands_num != 1)))
	{
		cmd_cd(arr);
		return (1);
	}
	else if (!(ft_strcmp(arr[0], "cd")))
		return (1);
	else
		return (0);
}

void	launch_exec(char **arr)
{
	int			ret;

	if ((ret = fork()) < 0)
		fork_error();
	signal(SIGINT, child_signal_handler);
	if (ret == 0)
	{
		if ((execve(arr[0], arr, g_env) < 0))
		{
			write(2, "\U0001F633 ", 5);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
			exit(1);
		}
		else
			exit(0);
	}
	waitpid(-1, NULL, 0);
}
