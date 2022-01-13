/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strarrlen.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 13:27:58 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 13:27:58 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int	ft_strarrlen(char **arr)
{
	size_t	len;
	size_t	idx;

	len = 0;
	if (!arr)
		return (len);
	idx = 0;
	while (arr[idx])
	{
		len++;
		idx++;
	}
	return (len);
}
