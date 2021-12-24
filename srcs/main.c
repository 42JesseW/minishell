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

//static int	create_inputrc_file(void)
//{
//	const char	*file_path;
//	const char	*home;
//
//	home = getenv("HOME");
//	if (!home)
//	{
//		ft_dprintf("create_inputrc_file: HOME not set");
//		return (SYS_ERROR);
//	}
//	file_path = ft_strjoin(home, "/.inputrc")
//}
//
//int	init_inputrc(void)
//{
//	/* if file does not exist, create */
//	if (access())
//}

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
	set_signals();
	shell = shell_init(envp);
	if (!shell)
		return (EXIT_FAILURE);
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
