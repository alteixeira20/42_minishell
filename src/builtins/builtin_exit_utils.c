/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jopedro- <jopedro-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:57:55 by jopedro-          #+#    #+#             */
/*   Updated: 2025/05/26 15:04:06 by jopedro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	parse_sign_and_whitespace(const char *str, int *i)
{
	int	sign;

	*i = 0;
	sign = 1;
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

int	check_valid_long(const char *str, long *code)
{
	int				i;
	int				sign;
	long			result;

	result = 0;
	sign = parse_sign_and_whitespace(str, &i);
	if (!str[i])
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((sign == 1 && result > (LONG_MAX - (str[i] - '0')) / 10)
			|| (sign == -1 && (unsigned long)result
				> ((unsigned long)LONG_MAX + 1 - (str[i] - '0')) / 10))
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (0);
	if (sign == 1)
		*code = result;
	else
		*code = result * sign;
	return (1);
}
