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

static int	convert(t_list **redir_root, t_list *redir_node, t_redir *node)
{
	(void)redir_root;
	(void)redir_node;
	(void)node;
	return (1);
}

/*
** convert_heredocs() looks for REDIR_DELIM
** redirects and converts these to REDIR_IN
** redirects pointing to a temp file.
**
** Since
*/

int	convert_heredocs(t_shell *shell)
{
	t_list	*cmd_node;
	t_node	*node;
	t_list	*redir_node;
	t_redir	*redir;

	cmd_node = shell->cmd_nodes;
	while (cmd_node)
	{
		node = (t_node *)cmd_node->content;
		redir_node = node->redir;
		while (redir_node)
		{
			redir = (t_redir *)redir_node->content;
			if (redir->type == REDIR_DELIM)
			{
				if (convert(&node->redir, redir_node, redir) == SYS_ERROR)
					return (SYS_ERROR);
			}
			redir_node = redir_node->next;
		}
		cmd_node = cmd_node->next;
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
**	2. It adds empty TOK_WORD tokens to REDIR + WORD combinations	// TODO
**	   - i.e. "< IN", which is TOK_LESS->TOK_WORD to TOK_WORD->TOK_LESS->TOK_WORD
**	   This is to help creating the CMD nodes during the grouping phase.
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
	if (group_tokens(shell, &tokens) == SYS_ERROR)
		return (0);

	nodes_print_stdout(shell->cmd_nodes);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	return (1);
}
