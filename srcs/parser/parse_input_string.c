/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_input_string.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 16:21:32 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 16:21:32 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	redir_is_type(t_token_type type)
{
	return (type == TOK_LESS || type == TOK_GREAT
		|| type == TOK_DLESS || type == TOK_DGREAT);
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
*/
int	valid_pipe_position(t_list *scan_from)
{
	t_token	*token_left;
	t_token	*token_right;

	if (!scan_from || !scan_from->next->next)
		return (PARSE_FAIL);
	token_left = ((t_token *)scan_from->content);
	token_right = ((t_token *)scan_from->next->next->content);
	(void)token_left;
	(void)token_right;
	return (1);
}

/*
** evaluate_tokens() will do the following:
**	1. removes spaces from the token list since
**	   they are no longer relevant and will only
**	   be annoying to work with further on.
**	3. Look for syntax errors, meaning successive
** 	   tokens that don't follow the grammar rules
**	   of the shell.
**
** // TODO resolve aliases ?
**
** RETURN
**  - 1 on success, 0 on parsing error, -1 on sys_error
*/

int	evaluate(t_list **tokens)
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;

	prev = NULL;
	node = *tokens;
	remove_spaces(tokens);
	while (node)
	{
		prev = node;
		token = (t_token *)node->content;
		if (token->type == TOK_PIPE)
		{
			if (!valid_pipe_position(prev))
				return (PARSE_FAIL);
		}
		node = node->content;
	}
	return (1);
}

/*
** TESTS // TODO resolving quotes:
**	- export TEST="cat -e";ls|"$TEST"
**	- export TEST="cat -e";ls|$TEST
*/

/*
** parse_input_string() checks the incoming string
** from readline() and converts it into a list of
** t_node structures. readline may return NULL if
** it finds an EOF. In this case this function returns
** 0 and breaks the read loop in main.
**
** It does this by:
**
**	1. 		lexing / tokenizing
**	1.1.	scanning
**			- generating simple tokens based on input_string
**				- TOK_WORD is already grouped here
**			- grouping single tokens like > or < into << >>
**	1.2		evaluating
**			- checking for syntax errors
**	2.		parsing
**	2.1.	normalization
**			- converting aliases to simple form
**  2.2		expansion
**			- expanding environment variables
**	2.2		aggregation
**			- grouping tokens into t_node structures
**	2.3		here-docs
**			- resolving here-docs into redirect nodes
**
** RETURN
**	- 0 if EOF, 1 if successful, -1 if sys_error
*/

int	parse_input_string(const char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)		// TODO print exit on EOF ?
		return (0);
	if (!(*input_string))	// if just an ENTER readline() returns "" empty string
		return (1);
	tokens = tokenize(input_string);
	if (!tokens || !evaluate(&tokens))	// TODO some exit code struct?
		return (0);

	token_display_stdout(tokens);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	(void)shell;
	return (1);
}
