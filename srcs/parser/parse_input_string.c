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

void	normalize_part(t_list **tokens, t_list *start, t_list *end)
{

}

t_list	*ft_lst_at(t_list *root, int idx)
{
	t_list	*traverse;

	traverse = root;
	while (traverse && idx > 0)
	{
		traverse = traverse->next;
		idx--;
	}
	return (traverse);
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
**	   This is to help creating the CMD nodes during the grouping phase.
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
*/

void	normalize(t_list **tokens)
{
	t_list	*node;
	t_list	*start;
	int		idx;

	idx = 0;
	start = NULL;
	node = *tokens;
	while (node)
	{
		if (!node->next || ((t_token *)node->next->content)->type == TOK_PIPE)
		{
			normalize_part(tokens, start, node);
			start = ft_lst_at(*tokens, idx + 2);
			if (!start)
				return ;
			node = start;
			idx += 2;
		}
		else
		{
			node = node->next;
			idx++;
		}
	}
}

/*
** environ_get() returns a pointer to the string
** held by the t_pair structure if key matches
** pair.key of an existing environment variable.
*/
const char	*environ_get(t_list *environ, const char *key)
{
	t_list	*traverse;
	t_pair	*pair;

	if (!environ || !key)
		return (NULL);
	traverse = environ;
	while (traverse)
	{
		pair = (t_pair *)traverse->content;
		if (ft_strncmp(pair->key, key, ft_strlen(key)) == 0)
			return (pair->val);
		traverse = traverse->next;
	}
	return (NULL);
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
	// TODO normalize (resolve aliases) ?

	token_display_stdout(tokens);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	return (1);
}
