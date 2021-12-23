/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   correct_dollar.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/18 18:04:32 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/18 18:04:32 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	is_fail_type(t_token_type type)
{
	return (type == TOK_DOLLAR || type == TOK_QUOTE || type == TOK_DQUOTE);
}

static int	correct(t_list *node, t_token *token)
{
	t_token	*next_token;

	if (!node->next)
		token->type = TOK_WORD;
	else
	{
		next_token = ((t_token *)node->next->content);
		if (next_token->type != TOK_WORD)
		{
			if (is_fail_type(next_token->type))
				return (PARSE_FAIL);
			token->type = TOK_WORD;
		}
	}
	return (SUCCESS);
}

/*
** correct_dollar() converts TOK_DOLLAR to TOK_WORD if
** it is not directly followed by a TOK_WORD. It also
** gives a PARSE_FAIL if the TOK_DOLLAR is succeeded by
** any token other than TOK_WORD or TOK_SPACE:
** - QUOTE tokens since ANSI-C Quoting is not supported:
** 	 https://www.gnu.org/software/bash/manual/html_node/ANSI_002dC-Quoting.html
** - BASHPID ($$)
** 	 https://www.gnu.org/software/bash/manual/html_node/Bash-Variables.html
**
** TESTS // TODO check this weird edge case
**	- $>OUT
*/
int	correct_dollar(t_list *tokens)
{
	t_list	*node;
	t_token	*token;

	node = tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == TOK_DOLLAR)
		{
			if (correct(node, token) == PARSE_FAIL)
			{
				dprintf(STDERR_FILENO, SHELL_NAME SYNTAX_ERR, token->token);
				return (PARSE_FAIL);
			}
		}
		node = node->next;
	}
	return (SUCCESS);
}
