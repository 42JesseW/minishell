/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   resolve_dollar.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/19 14:54:53 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/19 14:54:53 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static const char	**get_kv(t_shell *shell, t_list *node, const char *kv[2])
{
	kv[0] = ((t_token *)node->next->content)->token;
	kv[1] = (char *)environ_get(shell->environ, kv[0]);
	return (kv);
}

/*
** resolve() should do the following:
**	1. Relink the list
**	2. remove the TOK_DOLLAR node
**	3. modify TOK_WORD
**
** If the key is NULL or pair.val from
** the found environment variable is
** NULL, then just create an empty string.
*/
static int	resolve(t_list **tokens, t_list *from, const char *kv[2])
{
	t_list	*temp;
	t_list	*word;
	char	*token;

	if (!from)
	{
		temp = *tokens;
		*tokens = (*tokens)->next;
	}
	else
	{
		temp = from->next;
		from->next = temp->next;
	}
	word = temp->next;
	ft_lstdelone(temp, token_del);
	if (!kv[0] || !kv[1])
		token = ft_strdup("");
	else
		token = ft_strdup(kv[1]);
	if (!token)
		return (SYS_ERROR);
	free(((t_token *)word->content)->token);
	((t_token *)word->content)->token = token;
	return (1);
}

/*
** expand_dollar() searches for TOK_DOLLAR
** tokens. It expands the TOK_DOLLAR to its
** corresponding environment variable value
** in the following conditions:
**	1. It is not inside TOK_QUOTE tokens.
**	2. It can find an environment variable key
**	   matching the TOK_WORD following TOK_DOLLAR.
**
** Resolving the TOK_DOLLAR means the following:
**	1. The TOK_DOLLAR token is removed
**	2. The TOK_WORD value is changed to the value
**	   of the found environment variable.
**
** In the case of an unknown variable, the TOK_DOLLAR
** is still removed, but the value of TOK_WORD will
** become empty "". Also, TOK_DOLLAR is also expanded
** if its inside of a nested quote: "'$ENV'"
*/

int	resolve_dollar(t_shell *shell, t_list **tokens)	// TODO testcase
{
	t_list		*node;
	t_list		*prev;
	t_token		*token;
	t_quote		quote;
	const char	*kv[2];

	prev = NULL;
	node = *tokens;
	quote = (t_quote){.between = false, .type = TOK_QUOTE};
	while (node)
	{
		token = (t_token *)node->content;			// TODO discuss readability
		if (!quote.between && quote_is_type(false, &quote, token->type))
			quote.between = true;
		else if (quote.between && quote_is_type(true, &quote, token->type))
			quote.between = false;
		if ((!quote.between || (quote.between && quote.type == TOK_DQUOTE))
			&& token->type == TOK_DOLLAR)
		{
			if (resolve(tokens, prev, get_kv(shell, node, kv)) < 0)
				return (SYS_ERROR);
		}
		prev = node;
		node = node->next;
	}
	return (1);
}
