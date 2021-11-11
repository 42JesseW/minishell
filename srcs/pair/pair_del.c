/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pair_del.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:52:59 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:52:59 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pair_del(t_pair **pair)
{
	t_pair	*p;

	p = *pair;
	if (p)
	{
		free(p->key);
		free(p->val);
	}
	free(p);
	*pair = NULL;
}
