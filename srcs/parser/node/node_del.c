/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_del.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:21:18 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 12:21:18 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

void	node_del(t_node	**node)
{
	t_node	*p;

	if (!node)
		return ;
	p = *node;
	if (p)
	{
		ft_strarrfree(&p->cmd);
		redir_lst_del(&p->redir);
		free(p);
	}
	*node = NULL;
}
