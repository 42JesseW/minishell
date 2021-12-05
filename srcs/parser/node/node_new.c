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

/* Default constructor for t_node structure */
t_node	*node_new_def(void)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->redir = NULL;
	return (node);
}

/*
** Construct t_node with its properties initialised
** Ownership of t_list structure is transferred to node.
*/
t_node	*node_new_val(char **cmd, t_list *redir)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	if (cmd)
		node->cmd = ft_strarrdup(cmd);
	node->redir = redir;
	if ((cmd && !node->cmd) || (redir && !node->redir))
	{
		node_del(&node);
		return (NULL);
	}
	return (node);
}

/*
** Construct t_node using information from another node structure
** New node copies all information, including the t_list structure.
** This means ownership of pointers is kept with the original creator.
*/
void	*node_new_cpy(void *cpy)
{
	t_node	*node;
	t_node	*copy;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	copy = cpy;
	node->cmd = NULL;
	if (copy->cmd)
		node->cmd = ft_strarrdup(copy->cmd);
	node->redir = ft_lstmap(copy->redir, redir_new_cpy, redir_del);
	if ((copy->cmd && !node->cmd) && (copy->redir && !node->redir))
	{
		node_del(&node);
		return (NULL);
	}
	return (node);
}
