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
**			-
**	1.2		evaluating
**			- checking for syntax errors
**	2.		parsing
**	2.1.	normalization
**			- converting aliases to simple form
**  2.2		expansion
**			- expanding environment variables			// TODO use generic resolve_dollar function
**	2.2		aggregation
**			- grouping tokens into t_node structures
**	2.3		here-docs
**			- resolving here-docs into redirect nodes	// TODO use generic resolve_dollar function
*/

int	parse_input_string(const char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)		// TODO print exit on EOF ?
		return (0);
	if (!(*input_string))	// if just an ENTER readline() returns "" empty string
		return (1);
	tokens = tokenize(input_string);
	if (!tokens)
		return (0);

	token_display_stdout(tokens);

	// group 2 > into one >> node. same for 2 < into <<.
	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	(void)shell;
	return (1);
}
