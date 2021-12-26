/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   group_tokens.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 17:30:09 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/12/02 17:30:09 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	error_with_cleanup(t_list **tokens)
{
	ft_lstclear(tokens, token_del);
	return (SYS_ERROR);
}

/* initialize shell.cmd_nodes by creating a new empty node */
static int	init_cmd_nodes(t_list **cmd_node)
{
	t_list	*cmd;
	t_node	*node;

	node = node_new_def();
	if (!node)
		return (SYS_ERROR);
	cmd = ft_lstnew(node);
	if (!cmd)
	{
		node_del(node);
		return (SYS_ERROR);
	}
	*cmd_node = cmd;
	return (1);
}

static void	move_and_unlink_token(t_list **tokens, t_list **node)
{
	t_list	*unlinked;

	*node = (*node)->next;
	unlinked = ft_lstunlink(tokens, *tokens);
	ft_lstdelone(unlinked, token_del);
}

/*
** reset_cmd_node() does the following:
**	1. Check if we're at the last token
**	(true)
**		1. consume the last token so its added to
**		   to the node structure.
**	(false)
**		1. create a new node structure that can be
**		   filled with more tokens.
**	2. In all cases, the *cmd_node is added to the
**	   the list of nodes in shell.cmd_nodes
*/
static int	reset_cmd_node(t_shell *shell, t_list **cmd_node,
							t_list *token_node)
{
	t_list	*cmd;
	t_node	*node;

	if (!token_node->next)
	{
		if (consume_token((*cmd_node), token_node) == SYS_ERROR)
			return (SYS_ERROR);
	}
	cmd = *cmd_node;
	ft_lstadd_back(&shell->cmd_nodes, cmd);
	*cmd_node = NULL;
	if (token_node->next)
	{
		node = node_new_def();
		*cmd_node = ft_lstnew(node);
		if (!(*cmd_node) || !(*cmd_node)->content)
		{
			ft_lstdelone((*cmd_node), node_del);
			return (SYS_ERROR);
		}
	}
	return (1);
}

/*
** group_tokens() groups the t_token structures
** into a t_node structure for the executor.
** Redirects are put into the list t_node.redir.
** structures and the program, together with
** its arguments, is stored inside t_node.cmd.
**
** tokens are consumed when combined into a
** single structure. The TOK_PIPE acts as delimiter
** when consuming tokens.
*/
int	group_tokens(t_shell *shell, t_list **tokens)
{
	t_list	*cmd_node;
	t_list	*token_node;
	t_token	*token;

	if (init_cmd_nodes(&cmd_node) == SYS_ERROR)
		return (SYS_ERROR);
	token_node = *tokens;
	while (token_node)
	{
		token = (t_token *)token_node->content;
		if (!token_node->next || token->type == TOK_PIPE)
		{
			if (reset_cmd_node(shell, &cmd_node, token_node) == SYS_ERROR)
				return (error_with_cleanup(tokens));
		}
		else
		{
			if (consume_token(cmd_node, token_node) == SYS_ERROR)
				return (error_with_cleanup(tokens));
		}
		move_and_unlink_token(tokens, &token_node);
	}
	return (1);
}
