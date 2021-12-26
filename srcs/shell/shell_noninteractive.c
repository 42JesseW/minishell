/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_noninteractive.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/26 11:13:05 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/26 11:13:05 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	execute(t_shell *shell, char *arg)
{
	char	*input_string;

	input_string = arg;
	if (!input_string)
	{
		if (get_next_line(STDIN_FILENO, &input_string) == SYS_ERROR)
			return (EXIT_FAILURE);
	}
	if (parse_input_string(input_string, shell) == SYS_ERROR)
		return (EXIT_FAILURE);
	if (init_exe(shell) == SYS_ERROR)
		return (EXIT_FAILURE);
	if (!arg)
		free(input_string);
	shell_destroy(&shell);
	return (EXIT_SUCCESS);
}

int	shell_noninteractive(t_shell *shell, char **argv)
{
	if (!argv[1])
		return (execute(shell, NULL));
	if (ft_strcmp(argv[1], "-c") != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"%s: %s: unknown flag\n", SHELL_NAME, argv[1]);
		shell_destroy(&shell);
		return (EXIT_FAILURE);
	}
	if (ft_strcmp(argv[1], "-c") == 0 && !argv[2])
	{
		ft_dprintf(STDERR_FILENO, "%s: -c: option requires an argument\n",
			SHELL_NAME);
		shell_destroy(&shell);
		return (EXIT_FAILURE);
	}
	return (execute(shell, argv[2]));
}
