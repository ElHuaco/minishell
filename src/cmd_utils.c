/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonroy- <mmonroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 09:39:50 by aleon-ca          #+#    #+#             */
/*   Updated: 2020/10/23 18:20:16 by aleon-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			read_input_subshell(char **input, char c)
{
	char		buff[1];
	int			bytes_read;
	char		*temp;

	*input = ft_add_char(*input, '\n');
	while ((bytes_read = read(0, buff, 1)) && (buff[0] != c))
		*input = ft_add_char(*input, buff[0]);
	read(0, buff, 1);
	if (!bytes_read)
		write(2, ESUBEOF, ft_strlen(ESUBEOF));
	else
		*input = ft_add_char(*input, c);
	if ((temp = ft_strchr(*input, '$')))
	{
		if (((temp - *input) != 0) && (temp[-1] == '\\'))
			remove_backslash(input, temp - 1);
		else
			replace_var_in_str(input, temp + 1);
	}
	return (bytes_read);
}

int			quotes_handler(char **arg, int i)
{
	char	*ptr;
	int		ret;

	ret = 1;
	if ((ptr = smallest_non_zero(ft_strchr(arg[i], '"'),
		ft_strchr(arg[i], '\''))) && !(ft_strchr(ptr + 1, *ptr)))
		ret = read_input_subshell(arg + i, *ptr);
	if (!ret)
		return (0);
	return (1);
}

void		export_no_arg(char **temp)
{
	int i;

	i = 0;
	while (temp[i])
	{
		write(1, "declare -x ", 11);
		write(1, temp[i], ft_strlen(temp[i]));
		write(1, "\n", 1);
		i++;
	}
	free_double(temp);
}
