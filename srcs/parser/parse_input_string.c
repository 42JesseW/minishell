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

int	g_exit_code_sig = 0;

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
static int	get_tokenize_fail_exit(t_list **tokens)
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
	if (*tokens)
		ft_lstclear(tokens, token_del);
	return (exit_code);
}

static int	parse_fail_exit(t_shell *shell, t_list **tokens)
{
	if (*tokens)
		ft_lstclear(tokens, token_del);
	shell->exit_code = EXIT_PARSE_FAIL;
	if (g_exit_code_sig)
		g_exit_code_sig = 0;
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
	if (!tokens || !has_paired_quotes(input_string))
		return (get_tokenize_fail_exit(&tokens));
	if (!redir_merge(tokens) || !correct_dollar(tokens))
		return (parse_fail_exit(shell, &tokens));
	if (insert_merge_token(&tokens) == SYS_ERROR)
		return (SYS_ERROR);
	if (remove_spaces(&tokens) == NONFATAL)
		return (NONFATAL);
	if (resolve_dollar(shell, &tokens, false) == SYS_ERROR)
		return (SYS_ERROR);
	if (resolve_quotes(&tokens) == SYS_ERROR)
		return (SYS_ERROR);
	if (!validate_syntax(tokens))
		return (parse_fail_exit(shell, &tokens));
	normalize(&tokens);
	if (group_tokens(shell, &tokens) == SYS_ERROR)
		return (SYS_ERROR);
	return (create_redir_files(shell));
}
