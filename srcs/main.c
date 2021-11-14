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

typedef enum e_token_type
{
	TOK_DOLLAR,
	TOK_QUOTE,
	TOK_DQUOTE,
	TOK_WORD,
	TOK_SPACE,
	TOK_PIPE,
	TOK_LESS,
	TOK_DLESS,
	TOK_GREAT,
	TOK_DGREAT
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*token;
}	t_token;

/*
** tokenize_input_string() splits the input_string into
** smaller tokens with a meaning. While creating tokens,
** it checks if the order conforms to the shell grammar.
** The tokenizer may set shell.exit_code = 2 when a syntax
** error is found. // TODO discuss implementation of exit_code
*/

t_list	*tokenize_input_string(const char *input_string)
{

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
**			- checking for syntax errors
**	1.2		evaluating
**			- resolving here-docs
**	2.		parsing
**	2.1.	normalization
**			- converting aliases to simple form
**  2.2		expansion
**			- expanding environment variables
**	2.2		aggregation
**			- grouping tokens into t_node structures
*/

int	parse_input_string(const char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)		// TODO print exit on EOF ?
		return (0);
	tokens = tokenize_input_string(input_string);
	if (!tokens)
		return (0);
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
