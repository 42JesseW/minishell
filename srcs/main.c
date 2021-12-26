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

static const char	g_prompt_startup[] = "\n"
	"        ______       ____         " B " .════. " R "    @   @\n"
	"       / __/ /  ___ / / /_ __   " B " / .═\"═.`. " R "   \\\\v/\n"
	"      _\\ \\/ _ \\/ -_) / / // /  " B "  ║ ║ '\\ \\ \\ " R "_/ )\n"
	"     /___/_//_/\\__/_/_/\\_, /  ,- " B "\\ `═.' /.' " R " / \n"
	"                      /___/   '--- " B "`════' " R "----'\n"
	"\n"
	"          -- created by aheister & jevan-de --"
	"\n";

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

	shell = shell_init(envp);
	if (!shell)
		return (EXIT_FAILURE);
	if (!isatty(STDIN_FILENO) || argc > 1)
		return (execute_single(shell, argv));
	printf("%s\n", g_prompt_startup);
	input_string = readline(DEFAULT_PROMPT);
	while (input_string)
	{
		if (parse_input_string(input_string, shell) == SYS_ERROR)
			break ;
		add_history(input_string);
		if (init_exe(shell) == SYS_ERROR)
			break ;
		free(input_string);
		input_string = readline(DEFAULT_PROMPT);
	}
	shell_destroy(&shell);
	return (EXIT_SUCCESS);
}
