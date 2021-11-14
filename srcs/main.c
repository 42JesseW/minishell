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
	}
	shell_destroy(&shell);
	return (EXIT_SUCCESS);
}
