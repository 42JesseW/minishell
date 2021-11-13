/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:37:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:37:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static const char	g_prompt[] = "shelly3.2$ ";

/*
** A multiline command is defined as a command line
** where a single quote ' or a double quote " is not
** properly closed by a matching quote of the same kind.
*/
static bool	has_paired_quotes(const char *str)
{
	char	*ptr;
	int		idx;

	if (!str || !(*str))
		return (true);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '\'' || str[idx] == '\"')
		{
			ptr = ft_strchr(str + idx + 1, str[idx]);
			if (!ptr)
				return (false);
			if (!has_paired_quotes(ptr + 1))
				return (false);
			idx += (int)(ptr - str);
		}
		idx++;
	}
	return (true);
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
**	1. 		lexing
**	1.1.	scanning
**			- checking for syntax errors
**	1.2		evaluating
**			- resolving here-docs
**			- expanding environment variables
**	2.		parsing
**	2.1.	normalization
**			- converting aliases to simple form
**	2.2		aggregation
**			- grouping tokens into t_node structures
*/

int	parse_input_string(const char *input_string, t_shell *shell)
{
	if (!input_string)
		return (0);
	if (has_paired_quotes(input_string))
		return (SYS_ERROR);
	(void)shell;
	return (1);
}

/*
** main() has 4 main jobs:
**	1. checking arguments passed to the program
**	2. initialising the t_shell struct
**	3. passing output from the initial readline() call to the lexer
**	4. passing output from the parser to the executor
*/

int	main(int argc, char *argv[], const char *envp[])
{
	t_shell	*shell;
	char	*input_string;

	if (argc > 1 || argv[1])
		return (EXIT_SUCCESS);
	shell = shell_init(envp, &input_string);
	if (!shell)
		return (EXIT_FAILURE);
	while (input_string)
	{
		free(input_string);
		input_string = readline(g_prompt);
		if (!parse_input_string(input_string, shell))
			break ;
		printf("%s\n", input_string);
	}
	shell_destroy(&shell);
	return (EXIT_SUCCESS);
}
