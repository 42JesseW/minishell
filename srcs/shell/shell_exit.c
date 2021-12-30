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

	p = *shell;
	exit_code = p->exit_code;
	shell_destroy(shell);
	return (exit_code);
}
