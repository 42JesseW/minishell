/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   resolve_quotes.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/20 17:00:12 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/20 17:00:12 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	remove_token(t_list **tokens, t_list *rm)
{
	t_list	*traverse;

	traverse = *tokens;
	if (!traverse)
		return ;
	if (traverse == rm)
	{
		*tokens = traverse->next;
		ft_lstdelone(traverse, token_del);
	}
	else
	{
		while (traverse->next != rm)
			traverse = traverse->next;
		traverse->next = rm->next;
		ft_lstdelone(rm, token_del);
	}
}

static int	append_to_token(t_token *new_token, t_list *node)
{
	t_token	*token;
	char	*new_token_str;

	token = (t_token *)node->content;
	new_token_str = ft_strjoin(new_token->token, token->token);
	if (!new_token_str)
		return (SYS_ERROR);
	free(new_token->token);
	new_token->token = new_token_str;
	return (SUCCESS);
}

static int	insert_word(t_list **tokens, t_list *prev, t_list **ref,
						t_token *word)
{
	t_list	*node;

	node = ft_lstnew(word);
	if (!node)
		return (SYS_ERROR);
	if (!prev)
	{
		node->next = *tokens;
		*tokens = node;
	}
	else
	{
		node->next = prev->next;
		prev->next = node;
	}
	*ref = node;
	return (SUCCESS);
}

/*
** resolve() should do the following:
**	1. create a new TOK_WORD node where .token is
**	   created by joining all .token components from
**	   all token nodes in between the matching quote.
**	2. remove all intermediary token nodes.
**	3. return the new token to the caller
*/

static t_token	*resolve(t_list **tokens, t_list *start, t_token_type type)
{
	t_list	*node;
	t_list	*temp;
	t_token	*token;

	token = token_new_val(TOK_WORD, "");
	if (!token)
		return (NULL);
	node = start->next;
	remove_token(tokens, start);
	while (((t_token *)node->content)->type != type)
	{
		if (append_to_token(token, node) == SYS_ERROR)
		{
			token_del(token);
			return (NULL);
		}
		temp = node;
		node = node->next;
		remove_token(tokens, temp);
	}
	remove_token(tokens, node);
	return (token);
}

/*
** resolve_quotes() creates one TOK_WORD out of
** TOK_QUOTE / TOK_DQUOTE and everything inside.
**
** assumes the following:
**	1. If a TOK_DOLLAR is found, this can safely
**	   be converted to a string (only in TOK_QUOTE).
**	2. Every QUOTE token has a matching QUOTE token
**	   of the same type (has_paired_quotes).
*/

int	resolve_quotes(t_list **tokens)
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;
	t_token	*word;

	prev = NULL;
	node = *tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == TOK_QUOTE || token->type == TOK_DQUOTE)
		{
			word = resolve(tokens, node, token->type);
			if (!word || insert_word(tokens, prev, &node, word) == SYS_ERROR)
				return (SYS_ERROR);
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return (merge_word_tokens(tokens));
}
