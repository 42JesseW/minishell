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

/*
** main() has 4 main jobs:
**	1. checking arguments passed to the program
**	2. initialising the t_shell struct
**	3. passing output from the initial readline() call to the parser
**	4. passing output from the parser to the executor
*/

int	main(int argc, char *argv[], const char *envp[])
{
	t_shell	*shell;
	char	*input_string;

	if (argc > 1 || argv[1])
		return (EXIT_SUCCESS);
	shell = shell_init(envp);
	if (!shell)
		return (EXIT_FAILURE);
	input_string = readline(DEFAULT_PROMPT);
	while (input_string)
	{
		if (!parse_input_string(input_string, shell))
			break ;
		ft_lstclear(&shell->cmd_nodes, node_del);
		free(input_string);
		input_string = readline(DEFAULT_PROMPT);
	}
	shell_destroy(&shell);
	return (EXIT_SUCCESS);
}
