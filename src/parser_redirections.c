/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleon-ca <aleon-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:30:53 by aleon-ca          #+#    #+#             */
/*   Updated: 2020/10/21 13:35:55 by aleon-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_smallest_non_zero(char **ptr)
{
	char	*smallest[1];

	if ((ptr[0]) && (ptr[1]) && (ptr[2]))
	{
		*smallest = (ptr[0] < ptr[1]) ? ptr[0] : ptr[1];
		*smallest = (*smallest < ptr[2]) ? *smallest : ptr[2];
	}
	else if (!ptr[0] && (ptr[1]) && (ptr[2]))
		*smallest = (ptr[1] < ptr[2]) ? ptr[1] : ptr[2];
	else if (!ptr[1] && (ptr[0]) && (ptr[2]))
		*smallest = (ptr[0] < ptr[2]) ? ptr[0] : ptr[2];
	else if (!ptr[2] && (ptr[0]) && (ptr[1]))
		*smallest = (ptr[0] < ptr[1]) ? ptr[0] : ptr[1];
	else if ((ptr[0]) && !ptr[1] && !ptr[2])
		*smallest = ptr[0];
	else if (!ptr[0] && (ptr[1]) && !ptr[2])
		*smallest = ptr[1];
	else if (!ptr[0] && !ptr[1] && (ptr[2]))
		*smallest = ptr[2];
	else
		*smallest = 0;
	return (*smallest);
}

static void	lone_symbol_found(t_command_table *tab, int *i, char **ptr)
{
	char	*first_ptr;
	int		len;
	char	*temp;

	first_ptr = find_smallest_non_zero(ptr);
	temp = dup_till_symbol(tab->simple_commands[i[0]][i[1] + 1]);
	len = ft_strlen(temp);
	redir_files_updt(tab, i, temp, (first_ptr == ptr[2]) * 'A'
		+ (first_ptr == ptr[0]) * 'I' + (first_ptr == ptr[1]) * 'O');
	if (((size_t)len == ft_strlen(tab->simple_commands[i[0]][i[1] + 1]))
		&& (temp))
		resize_arr_skip_pos(&tab->simple_commands[i[0]], i[1] + 1);
	else if ((temp))
	{
		temp = ft_strdup(tab->simple_commands[i[0]][i[1] + 1] + len);
		free(tab->simple_commands[i[0]][i[1] + 1]);
		tab->simple_commands[i[0]][i[1] + 1] = temp;
	}
	resize_arr_skip_pos(&tab->simple_commands[i[0]], i[1]);
}

static void	lone_symbol_in_word(t_command_table *tab, int *i, char *str, int t)
{
	int		len;

	free(str);
	str = dup_till_symbol(tab->simple_commands[i[0]][i[1] + 1]);
	len = ft_strlen(str);
	redir_files_updt(tab, i, str, t);
	if ((size_t)len == ft_strlen(tab->simple_commands[i[0]][i[1] + 1]))
		resize_arr_skip_pos(&tab->simple_commands[i[0]], i[1] + 1);
	else
	{
		str = ft_strdup(tab->simple_commands[i[0]][i[1] + 1] + len);
		free(tab->simple_commands[i[0]][i[1] + 1]);
		tab->simple_commands[i[0]][i[1] + 1] = str;
	}
}

static void	redirect_word_found(t_command_table *tab, int *i, char **ptr)
{
	char	*first_ptr;
	int		len;
	char	*temp;

	first_ptr = find_smallest_non_zero(ptr);
	temp = dup_till_symbol(first_ptr + 1);
	len = ft_strlen(temp);
	first_ptr == ptr[2] ? first_ptr[-1] = '\0' : 0;
	first_ptr[0] = '\0';
	if ((len == 0) && !(first_ptr[1]))
		lone_symbol_in_word(tab, i, temp, (first_ptr == ptr[2]) * 'A'
			+ (first_ptr == ptr[0]) * 'I' + (first_ptr == ptr[1]) * 'O');
	else
	{
		redir_files_updt(tab, i, temp, (first_ptr == ptr[2]) * 'A'
			+ (first_ptr == ptr[0]) * 'I' + (first_ptr == ptr[1]) * 'O');
		temp = ft_strdup(first_ptr + 1 + len);
		first_ptr = ft_strjoin(tab->simple_commands[i[0]][i[1]], temp);
		free(tab->simple_commands[i[0]][i[1]]);
		if (temp)
			free(temp);
		tab->simple_commands[i[0]][i[1]] = first_ptr;
		if (!(tab->simple_commands[i[0]][i[1]][0]))
			resize_arr_skip_pos(&tab->simple_commands[i[0]], i[1]);
	}
}

int			set_redirection_arr(t_command_table *tab, int *i)
{
	char	*str;
	char	*ptr[3];
	char	**quotpos;

	i[1] = -1;
	while ((str = tab->simple_commands[i[0]][++(*(i + 1))]))
	{
		quotpos = set_quotpos_arr(str);
		ptr[0] = ft_strchr__quots(quotpos, str, '<');
		ptr[1] = ft_str1chr__quots(quotpos, str, '>');
		ptr[2] = ft_str2chr__quots(quotpos, str, '>');
		free(quotpos);
		if ((ptr[0]) || (ptr[1]) || (ptr[2]))
		{
			if (((ft_strlen(str) == 1) && ((ptr[0]) || (ptr[1])))
				|| ((ft_strlen(str) == 2) && (ptr[2])))
				lone_symbol_found(tab, i, ptr);
			else
				redirect_word_found(tab, i, ptr);
			if ((is_parser_error(tab, i[0])))
				return (1);
			i[1] = -1;
		}
	}
	return (0);
}
