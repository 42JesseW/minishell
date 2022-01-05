/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   insert_merge_tok.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/31 11:21:37 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/31 11:21:37 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* Insert new merge token and move current loop pointers */
static int	insert(t_list **tokens, t_list **node, t_list **prev)
{
	t_list	*merge_node;
	t_token	*token;
	int		idx;

	token = token_new_val(TOK_MERGE, "");
	if (!token)
		return (SYS_ERROR);
	merge_node = ft_lstnew(token);
	if (!merge_node)
	{
		token_del(token);
		return (SYS_ERROR);
	}
	idx = ft_lstget_idx(*tokens, *node);
	ft_lstinsert(tokens, merge_node, idx);
	*node = merge_node->next->next;
	*prev = merge_node->next;
	return (SUCCESS);
}

bool	is_quote_tok(t_token_type type)
{
	return (type == TOK_DQUOTE || type == TOK_QUOTE);
}

bool	from_quote(t_quote *quote, t_list *node, t_list *prev)
{
	t_token	*cur_token;
	t_token	*prev_token;

	if (!node->next)
		return (false);
	cur_token = (t_token *)node->content;
	if (quote->between && is_quote_tok(cur_token->type))
	{
		prev_token = (t_token *)prev->content;
		return (prev_token->type == TOK_WORD
			|| prev_token->type == TOK_DOLLAR
			|| is_quote_tok(prev_token->type));
	}
	return (false);
}

bool	from_other(t_quote *quote, t_list *node, t_list *prev)
{
	t_token	*cur_token;
	t_token	*prev_token;

	prev_token = (t_token *)prev->content;
	if (!quote->between && is_quote_tok(prev_token->type))
	{
		cur_token = (t_token *)node->content;
		return (cur_token->type == TOK_WORD || cur_token->type == TOK_DOLLAR);
	}
	return (false);
}

/*
** Checks if a valid insert position for merge token. For example:
**	"word"(x)"word" || 'word'(x)'word'
**	"word"(x)'word' || 'word'(x)"word"
**	'word'(x)word   || word(x)'word'
**	...
**
*/
bool	is_insert_pos(t_quote *quote, t_list *node, t_list *prev)
{
	if (!prev)
		return (false);
	return (from_quote(quote, node, prev) || from_other(quote, node, prev));
}

/*
** Insert special tokens in the case that an opening
** quote is directly followed by a closing quote:
** - export DOCK=dock ; cat "$DOCK"'er'"file"
**
** This is to prevent word splitting. The resolve_quotes
** function will use these to merge back TOK_WORD tokens
** if necessary.
*/
int	insert_merge_token(t_list **tokens)
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;
	t_quote	quote;

	prev = NULL;
	node = *tokens;
	quote = (t_quote){.between = false, .type = TOK_QUOTE};
	while (node)
	{
		token = (t_token *)node->content;
		if (!quote.between && quote_is_type(false, &quote, token->type))
			quote.between = true;
		else if (quote.between && quote_is_type(true, &quote, token->type))
			quote.between = false;
		if (is_insert_pos(&quote, node, prev))
		{
			if (insert(tokens, &node, &prev) == SYS_ERROR)
				return (SYS_ERROR);
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return (SUCCESS);
}
