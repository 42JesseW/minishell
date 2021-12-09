/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_merge.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/17 13:31:25 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/17 13:31:25 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* merge two redir tokens. fails if two different redir types */
static int	merge(t_list *redir1, t_list *redir2)
{
	t_token	*token1;
	t_token	*token2;

	if (redir1 == redir2)
		return (SUCCESS);
	token1 = (t_token *)redir1->content;
	token2 = (t_token *)redir2->content;
	if (token1->type != token2->type)
		return (PARSE_FAIL);
	free(token1->token);
	if (token2->type == TOK_GREAT)
	{
		token1->type = TOK_DGREAT;
		token1->token = ft_strdup(">>");
	}
	else
	{
		token1->type = TOK_DLESS;
		token1->token = ft_strdup("<<");
	}
	token2->token = NULL;
	redir1->next = redir2->next;
	ft_lstdelone(redir2, redir_del);
	return (SUCCESS);
}

static int	scan_redirs(t_list *scan_from, t_list **merge_node)
{
	t_token	*token;
	t_list	*prev;
	t_list	*node;
	int		count;

	count = 0;
	prev = NULL;
	node = scan_from;
	while (node)
	{
		token = (t_token *)node->content;
		if (!is_redir_type(token->type, REDIR_SINGLE))
			break ;
		prev = node;
		node = node->next;
		count++;
	}
	*merge_node = prev;
	return (count);
}

/*
** redir_merge() groups tokens like >
** or < into << or >> tokens. It checks the
** following syntax errors:
**	| >>>
**	| <<<
**	| <>
**	| ><
**
*/
int	redir_merge(t_list *tokens)
{
	t_list	*merge_node;
	t_list	*node;
	t_token	*token;
	int		count;

	node = tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (is_redir_type(token->type, REDIR_SINGLE))
		{
			count = scan_redirs(node, &merge_node);
			if (count > 2 || (count == 2 && !merge(node, merge_node)))
			{
				dprintf(STDERR_FILENO, SHELL_NAME SYNTAX_ERR, token->token);
				return (PARSE_FAIL);
			}
		}
		node = node->next;
	}
	return (SUCCESS);
}
