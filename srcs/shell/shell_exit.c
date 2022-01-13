/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_exit.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/30 14:13:25 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/30 14:13:25 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	shell_exit(t_shell **shell)
{
	t_shell	*p;
	int		exit_code;
	bool	shell_exit;

	p = *shell;
	exit_code = p->exit_code;
	shell_exit = p->shell_exit;
	shell_destroy(shell);
	if (errno == 0 && isatty(STDIN_FILENO) && !shell_exit)
		printf("exit\n");
	return (exit_code);
}
