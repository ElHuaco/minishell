/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digit_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonroy- <mmonroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 10:35:53 by aleon-ca          #+#    #+#             */
/*   Updated: 2020/10/21 12:32:29 by aleon-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		digit_number(int n, int basesize)
{
	int i;

	if (n < 0)
		n *= -1;
	i = 1;
	while ((n / basesize) > 0)
	{
		n = n / basesize;
		i++;
	}
	return (i);
}
