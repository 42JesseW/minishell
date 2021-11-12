/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_new.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 13:09:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 13:09:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_node	*node_new_def(void)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->next = NULL;
	node->redir = NULL;
	return (node);
}

t_node	*node_new_val(char **cmd, t_redir *redir)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	if (cmd)
		node->cmd = ft_strarrdup(cmd);
	node->redir = redir_lst_dup(redir);
	if ((cmd && !node->cmd) || (redir && !node->redir))
	{
		node_del(&node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_node	*node_new_cpy(t_node *cpy)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	if (cpy->cmd)
		node->cmd = ft_strarrdup(cpy->cmd);
	node->redir = redir_lst_dup(cpy->redir);
	if ((cpy->cmd && !node->cmd) && (cpy->redir && !node->redir))
	{
		node_del(&node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}
