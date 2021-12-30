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

// TODO must be able to loop multiple times (i.e. cat << EOF {pwd pwd} EOF | ./minishell
static int	execute(t_shell **shell, char *arg)
{
	t_shell	*p;
	char	*input_string;

	p = *shell;
	input_string = arg;
	if (!input_string)
	{
		if (get_next_line(STDIN_FILENO, &input_string) == SYS_ERROR)
			return (EXIT_FAILURE);
	}
	if (parse_input_string(input_string, p) == SYS_ERROR)
		return (EXIT_FAILURE);
	if (init_exe(p) == SYS_ERROR)
		return (EXIT_FAILURE);
	if (!arg)
		free(input_string);
	return (shell_exit(shell));
}

int	shell_noninteractive(t_shell **shell, char **argv)
{
	t_shell	*p;

	p = *shell;
	if (!argv[1])
	{
		errno = 0;
		return (execute(shell, NULL));
	}
	if (ft_strcmp(argv[1], "-c") != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"%s: %s: unknown flag\n", SHELL_NAME, argv[1]);
		p->exit_code = EXIT_FAILURE;
		return (shell_exit(shell));
	}
	if (ft_strcmp(argv[1], "-c") == 0 && !argv[2])
	{
		ft_dprintf(STDERR_FILENO, "%s: -c: option requires an argument\n",
			SHELL_NAME);
		p->exit_code = EXIT_FAILURE;
		return (shell_exit(shell));
	}
	return (execute(shell, argv[2]));
}
