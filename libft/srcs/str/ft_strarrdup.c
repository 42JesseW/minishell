/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strarrdup.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 13:28:17 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 13:28:17 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	**ft_strarrdup(char **arr)
{
	char	**duparr;
	int		idx;

	duparr = (char **)malloc(sizeof(char *) * (ft_strarrlen(arr) + 1));
	if (!duparr)
		return (NULL);
	idx = 0;
	while (arr[idx])
	{
		duparr[idx] = ft_strdup(arr[idx]);
		if (!duparr[idx])
		{
			ft_strarrfree(&duparr);
			return (NULL);
		}
		idx++;
	}
	duparr[idx] = NULL;
	return (duparr);
}
