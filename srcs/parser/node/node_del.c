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

#include <minishell.h>

void	node_del(void *node)
{
	t_node	*p;

	p = node;
	if (p)
	{
		ft_strarrfree(&p->cmd);
		ft_lstclear(&p->redir, redir_del);
		free(p);
	}
}
