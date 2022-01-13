/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   remove_spaces.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/17 17:15:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/17 17:15:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* remove and free space node and link dependent nodes */
static void	rm(t_list **tokens, t_list **node, t_list **prev)
{
	t_list	*temp;

	temp = *node;
	if (!(*prev))
		*tokens = temp->next;
	else
		(*prev)->next = temp->next;
	(*node) = temp->next;
	ft_lstdelone(temp, token_del);
}

/* remove spaces from the token list ignoring those between quotes. */
int	remove_spaces(t_list **tokens)
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
		if (!quote.between && token->type == TOK_SPACE)
			rm(tokens, &node, &prev);
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return (*tokens != NULL);
}
