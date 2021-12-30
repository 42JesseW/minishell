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

// TODO check in container "ls -la | wc -l" leaks in group_tokens
// TODO "env | grep "a" | cat -e" blijft nu hangen

/*
** The tokenizer can fail for 3 reasons:
**	1. input_string is empty
**	2. input_string has non matching quotes (i.e. multiline command)
**	3. malloc fails to allocate for tokens
**
** The first we can discard since parse_input_string
** already checks for this. For option 3, malloc will
** set {errno} to reflect the error.
*/
static int	get_tokenize_fail_exit(void)
{
	int		exit_code;

	exit_code = NONFATAL;
	if (errno != 0)
	{
		exit_code = SYS_ERROR;
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "system",
			strerror(errno));
	}
	else
		ft_dprintf(STDERR_FILENO, SHELL_NAME SYNTAX_ERR, "multiline");
	return (exit_code);
}

static int	parse_fail_exit(t_shell *shell)
{
	shell->exit_code = EXIT_PARSE_FAIL;
	return (NONFATAL);
}

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
*/
int	parse_input_string(char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)
		return (NONFATAL);
	if (!(*input_string))
		return (SUCCESS);
	tokens = tokenize(input_string);
	if (!tokens)
		return (get_tokenize_fail_exit());
	if (!redir_merge(tokens) || !correct_dollar(tokens))
		return (parse_fail_exit(shell));
	remove_spaces(&tokens);
	if (!validate_syntax(tokens))
		return (parse_fail_exit(shell));
	if (resolve_dollar(shell, &tokens) == SYS_ERROR)
		return (SYS_ERROR);
	if (resolve_quotes(&tokens) == SYS_ERROR)
		return (SYS_ERROR);
	normalize(&tokens);
	if (group_tokens(shell, &tokens) == SYS_ERROR)
		return (SYS_ERROR);
	if (create_redir_files(shell) == SYS_ERROR)
		return (SYS_ERROR);
	return (SUCCESS);
}
