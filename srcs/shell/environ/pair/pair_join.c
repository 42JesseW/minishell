/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pair_join.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/06 17:14:45 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/06 17:14:45 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Join the key and value part of pair
**	  back into a string together with a
**	  '=' character if value is not NULL.
**	  {len_val} starts at -1 to make sure
**	  that if {pair.val} is NULL, no room
**	  is made for the '='  character.
*/

char	*pair_join(t_pair *pair)
{
	char	*kv;
	size_t	len_key;
	size_t	len_val;

	len_val = -1;
	len_key = ft_strlen(pair->key);
	if (pair->val)
		len_val = ft_strlen(pair->val);
	kv = (char *)ft_calloc(len_key + len_val + 2, sizeof(char));
	if (!kv)
		return (NULL);
	ft_strcpy(kv, pair->key);
	if (pair->val)
	{
		ft_strcpy(kv + len_key, "=");
		ft_strcpy(kv + len_key + 1, pair->val);
	}
	return (kv);
}
