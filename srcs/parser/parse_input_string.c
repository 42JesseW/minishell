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

static bool	quote_is_type(t_token_type type)
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
	if (!(tok_left->type == TOK_WORD || quote_is_type(tok_left->type)))
		return (PARSE_FAIL);
	if (tok_right->type == TOK_PIPE)
		return (PARSE_FAIL);
	return (1);
}

/*
** normalize() has two main jobs:
**	1. It converts aliased combinations to a
**	   standard configuration. These are some examples:
**	   [<] [FILE] [cat] 					-> [cat] [<] [FILE]
**	   [<] [FILE1] [>] [FILE2] [cat]		-> [cat] [<] [FILE1] [>] [FILE2]
**	   [>] [FILE1] [cat] [<] [FILE2]		-> [cat] [>] [FILE1] [<] [FILE2]
**	   [<] [FILE] [cat] [-e]				-> [cat] [-e] [<] [FILE]
**	   [<] [FILE1] [cat] [-e] [>] [FILE2]	-> [cat] [-e] [<] [FILE1] [>] [FILE2]
**	   [cat] [-e] [cat] [<] [FILE1] [>] [FILE2] [>] [FILE3] [cat]	-> [cat -e cat < FILE1 > FILE2 > FILE3]
**	2. It adds empty TOK_WORD tokens to REDIR + WORD combinations
**	   - i.e. "< IN", which is TOK_LESS->TOK_WORD to TOK_WORD->TOK_LESS->TOK_WORD
**	   This is to help the parser create the CMD nodes during the parsing phase.
**
** in all cases the actual program(s) (with flags) is (are)
** moved to the front of the total command line. This is to
** keep the order correct.
**
** since a command line can be broken up into sub
** commands using pipes, and normalization only applies
** to redir nodes, normalize() uses normalize_part() to
** normalize each sub command line.
**	// TODO each sub command can be normalized recursively
**	// TODO must be done after resolving environment variables
*/

void	normalize(t_list **tokens)
{
	(void)tokens;
}

/*
** evaluate_tokens() will do the following:
**	1. removes spaces from the token list since
**	   they are no longer relevant and will only
**	   be annoying to work with further on.
**	2. Look for syntax errors, meaning successive
** 	   tokens that don't follow the grammar rules
**	   of the shell.
**
** // TODO resolve aliases ?
**
** RETURN
**  - 1 on success, 0 on parsing error, -1 on sys_error
*/

int	evaluate(t_list **tokens)	// TODO testcase
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;

	prev = NULL;
	remove_spaces(tokens);
	node = *tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == TOK_PIPE)
		{
			if (!valid_pipe_position(prev))
				return (PARSE_FAIL);
		}
		prev = node;
		node = node->next;
	}
	return (1);
}

/*
** TESTS // TODO resolving quotes:
**	- export TEST="cat -e";ls|"$TEST"
**	- export TEST="cat -e";ls|$TEST
**	- ""<OUT2	-> char **cmd should not be NULL
**	- <OUT2		-> char **cmd should be NULl because empty TOK_WORD
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
**			- Checking for syntax errors (*1)
**				- multi-line commands
**				- unknown redir tokens
**				- unknown dollar implementation
**	1.2		evaluating
**			- removing TOK_SPACE tokens
**			- checking for syntax errors (*2)
**	2.		parsing
**  2.1		expansion
**			- expanding environment variables
**	2.2.	normalization
**			- converting aliases to simple form
**	2.2		aggregation
**			- grouping tokens into t_node structures
**	2.3		here-docs
**			- resolving REDIR_DELIM into REDIR_IN nodes
**				- resolving TOK_DOLLAR inside as well
**
** RETURN
**	- 0 if EOF, 1 if successful, -1 if sys_error
*/

int	parse_input_string(const char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)					// TODO print exit on EOF ?
		return (0);
	if (!(*input_string))				// if just an ENTER readline() returns "" empty string
		return (1);
	tokens = tokenize(input_string);
	if (!tokens)						// TODO some exit code struct?
		return (0);
	if (!redirs_merge(tokens))			// TODO some exit code struct?
		return (0);
	if (!correct_dollar(tokens))		// TODO some exit code struct?
		return (0);
	if (!evaluate(&tokens))
		return (0);						// TODO some exit code struct?
	token_display_stdout(tokens);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	(void)shell;
	return (1);
}
