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

static void	swap_ptr(t_list **tokens, t_list **word, t_list **next_node)
{
	*word = (*next_node)->next;
	ft_lstunlink(tokens, *next_node);
	ft_lstdelone(*next_node, token_del);
	*next_node = *word;
}

static char	*get_exit_code(int shell_exit_code)
{
	int	exit_code;

	exit_code = shell_exit_code;
	if (g_exit_code_sig)
	{
		exit_code = g_exit_code_sig;
		g_exit_code_sig = 0;
	}
	return (ft_itoa(exit_code));
}

/*
** resolve() should do the following:
**	1. unlink the TOK_DOLLAR node
**	2. relink {node} pointer for outer loop
**	3. modify TOK_WORD
**
** If the key is NULL or pair.val from
** the found environment variable is
** NULL, then just create an empty string.
*/
static int	resolve(t_shell *shell, t_list **tokens, t_list **node)
{
	t_token		*token;
	t_list		*word;
	const char	*kv[2];
	char		*token_string;

	swap_ptr(tokens, &word, node);
	token = (t_token *)word->content;
	kv[0] = token->token;
	if (ft_strcmp(kv[0], "?") == 0)
		token_string = get_exit_code(shell->exit_code);
	else
	{
		kv[1] = environ_get(shell->environ, kv[0]);
		if (!kv[0] || !kv[1])
			token_string = ft_strdup("");
		else
			token_string = ft_strdup(kv[1]);
	}
	if (!token_string)
		return (SYS_ERROR);
	free(token->token);
	token->token = token_string;
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

int	resolve_dollar(t_shell *shell, t_list **tokens, bool in_heredoc)
{
	t_list	*node;
	t_token	*token;
	t_quote	quote;

	node = *tokens;
	quote = (t_quote){.between = false, .type = TOK_QUOTE};
	while (node)
	{
		token = (t_token *)node->content;
		if (!quote.between && quote_is_type(false, &quote, token->type))
			quote.between = true;
		else if (quote.between && quote_is_type(true, &quote, token->type))
			quote.between = false;
		if ((in_heredoc || !quote.between
				|| (quote.between && quote.type == TOK_DQUOTE))
			&& token->type == TOK_DOLLAR)
		{
			if (resolve(shell, tokens, &node) < 0)
				return (SYS_ERROR);
		}
		else
			node = node->next;
	}
	return (SUCCESS);
}
