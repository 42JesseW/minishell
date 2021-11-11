/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strndup.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/10 11:45:21 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/10 11:45:21 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strndup(const char *str, size_t n)
{
	char	*dup;
	size_t	idx;

	if (!str)
		return (NULL);
	dup = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	idx = 0;
	while (str[idx] && idx < n)
	{
		dup[idx] = str[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}
