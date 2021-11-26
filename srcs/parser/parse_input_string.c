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

/*
**	EXAMPLES:
**	- [<] [word] [word]
**	- [word] [<] [word] [word]
**	- [<] [word] [word] [>] [word] [word]
**
**	TESTS: // TODO
**	- [cat < Makefile -e > FILE1 -b > FILE2]
**	- [cat -e OUT1 < OUT2 OUT]
**	- [< OUT cat -e]
*/



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
**				- pipe position check
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

int	parse_input_string(char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)					// TODO print exit on EOF ?
		return (0);
	if (!(*input_string))				// if just an ENTER readline() returns "" empty string
		return (1);
	tokens = tokenize(input_string);
	if (!tokens)						// TODO some exit code struct?
		return (0);
	if (!redir_merge(tokens))			// TODO some exit code struct?
		return (0);
	if (!correct_dollar(tokens))		// TODO some exit code struct?
		return (0);
	remove_spaces(&tokens);
	if (!validate_pipes(tokens))		// TODO some exit code struct?
		return (0);
	if (resolve_dollar(shell, &tokens) == SYS_ERROR)
		return (0);						// TODO some exit code struct?
	if (resolve_quotes(&tokens) == SYS_ERROR)
		return (0);
	normalize(&tokens);

	token_display_stdout(tokens);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	return (1);
}
