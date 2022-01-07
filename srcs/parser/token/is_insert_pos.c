/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_insert_pos.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/06 14:12:05 by jessevander...#+#    #+#                 */
/*   Updated: 2022/01/06 14:12:05 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	is_quote_tok(t_token_type type)
{
	return (type == TOK_DQUOTE || type == TOK_QUOTE);
}

static bool	from_quote(t_quote *quote, t_list *node, t_list *prev)
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

static bool	from_other(t_quote *quote, t_list *node, t_list *prev)
{
	t_token	*cur_token;
	t_token	*prev_token;

	cur_token = (t_token *)node->content;
	prev_token = (t_token *)prev->content;
	if (!quote->between && is_quote_tok(prev_token->type))
		return (cur_token->type == TOK_WORD || cur_token->type == TOK_DOLLAR);
	if (cur_token->type == TOK_DOLLAR && prev_token->type == TOK_WORD)
		return (true);
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
