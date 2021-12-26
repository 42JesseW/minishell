/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate_syntax.c                                   :+:    :+:           */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/19 12:15:37 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/19 12:15:37 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	is_quote(t_token_type type)
{
	return (type == TOK_QUOTE || type == TOK_DQUOTE);
}

/*
** DESCRIPTION
**	valid_pipe_position() checks if the pipe placed
**	in a valid position. If a pipe is placed at the
**	start of the token list or there is no token on
**	the right side of the the token, it is immediately
**	flagged as incorrect.
**
** PARAMERS
**	- {scan_from} : points to the token before the pipe
**
** A valid pipe is as follows:
**	[command]|[command]
**
** Example invalid positions are:
**	|
**	[command]|
**	>|
**	>>|
**	||
**
** TESTS // TODO
**	- export TEST="cat";ls|$TEST -e
*/
static int	valid_pipe_position(t_list *scan_from)
{
	t_token	*tok_left;
	t_token	*tok_right;

	if (!scan_from || !scan_from->next->next)
		return (PARSE_FAIL);
	tok_left = ((t_token *)scan_from->content);
	tok_right = ((t_token *)scan_from->next->next->content);
	if (!(tok_left->type == TOK_WORD || is_quote(tok_left->type)))
		return (PARSE_FAIL);
	if (tok_right->type == TOK_PIPE)
		return (PARSE_FAIL);
	return (1);
}

static bool	redir_has_word(t_list *token_node)
{
	t_token	*word;

	if (!token_node->next)
		return (false);
	word = (t_token *)token_node->next->content;
	return (word->type != TOK_PIPE);
}

/*
** validate_syntax() will Look for syntax errors, meaning
** successive tokens that don't follow the grammar
** rules of the shell.
*/

int	validate_syntax(t_list *tokens)
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;

	prev = NULL;
	node = tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if ((token->type == TOK_PIPE && !valid_pipe_position(prev))
			|| (is_redir_type(token->type, REDIR_ALL) && !redir_has_word(node)))
		{
			dprintf(STDERR_FILENO, SHELL_NAME SYNTAX_ERR, token->token);
			return (PARSE_FAIL);
		}
		prev = node;
		node = node->next;
	}
	return (1);
}
