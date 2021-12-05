/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   consume_token.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 17:31:50 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/12/02 17:31:50 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** is_redir_file() checks if TOK_WORD does not
** belong to redir.file. This would be the case
** if redir.file is still NULL since every valid
** redirect must have a corresponding TOK_WORD.
**
** If above condition is true, *last_redir will
** point to the the last redir structure in the
** list held by {node}.
*/
static bool	is_redir_file(t_node *node, t_redir **last_redir)
{
	t_redir	*redir;
	t_list	*last;

	redir = NULL;
	last = ft_lstlast(node->redir);
	if (last)
	{
		redir = (t_redir *)last->content;
		if (!redir->file)
		{
			*last_redir = redir;
			return (true);
		}
	}
	return (false);
}

static char	**allocate_new_cmd(t_list *token_node)
{
	t_list	*node;
	int		word_amount;
	char	**cmd;

	word_amount = 0;
	node = token_node;
	while (node && ((t_token *)node->content)->type == TOK_WORD)
	{
		node = node->next;
		word_amount++;
	}
	cmd = (char **)malloc(sizeof(char *) * (word_amount + 1));
	if (!cmd)
		return (NULL);
	while (word_amount >= 0)
	{
		cmd[word_amount] = NULL;
		word_amount--;
	}
	return (cmd);
}

static int	consume_word_token(t_node *node, t_list *token_node, int *word_idx)
{
	t_token	*token;
	t_redir	*redir;

	redir = NULL;
	token = (t_token *)token_node->content;
	if (is_redir_file(node, &redir))
	{
		redir->file = ft_strdup(token->token);
		if (!redir->file)
			return (SYS_ERROR);
	}
	else
	{
		if (!node->cmd)
		{
			node->cmd = allocate_new_cmd(token_node);
			if (!node->cmd)
				return (SYS_ERROR);
		}
		node->cmd[(*word_idx)] = ft_strdup(token->token);
		if (!node->cmd[(*word_idx)])
			return (SYS_ERROR);
		(*word_idx)++;
	}
	return (1);
}

/* token.type (t_token_type) maps directly to node.type (t_redir_type). */
static int	consume_redir_token(t_node *node, t_token *token)
{
	t_list	*redir_node;
	t_redir	*redir;

	redir = redir_new_val((t_redir_type)token->type, NULL, -1);
	if (!redir)
		return (SYS_ERROR);
	redir_node = ft_lstnew(redir);
	if (!redir_node)
	{
		redir_del(redir);
		return (SYS_ERROR);
	}
	ft_lstadd_back(&node->redir, redir_node);
	return (1);
}

/*
** consume_token() adds data to the new cmd_node
** based on the token type. TOK_WORD tokens are
** added to node.cmd and redirect tokens are added
** to the list node.redir.
**
** For TOK_WORD nodes the following is checked:
**	1.	Is node.cmd already allocated
**	(false)
**		1. Scan forward and count TOK_WORD.
**		2. Allocate for that many strings
**		3. Copy first TOK_WORD into node.cmd
**		4. Increment word_idx
**	(true)
**		1. Copy TOK_WORD into node.cmd
**		2. Increment word_idx
*/
int	consume_token(t_list *cmd_node, t_list *token_node)
{
	t_token		*token;
	t_node		*node;
	static int	word_idx = 0;

	node = (t_node *)cmd_node->content;
	if (!node->cmd && !node->redir)
		word_idx = 0;
	token = (t_token *)token_node->content;
	if (is_redir_type(token->type, REDIR_ALL))
	{
		if (consume_redir_token(node, token) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (token->type == TOK_WORD)
	{
		if (consume_word_token(node, token_node, &word_idx) == SYS_ERROR)
			return (SYS_ERROR);
	}
	return (1);
}
