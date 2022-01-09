/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/23 16:58:55 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/23 16:58:55 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	g_exit_code_sig = 1;
	rl_redisplay();
	rl_done = 1;
}

void	sigint_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	g_exit_code_sig = 1;
	rl_on_new_line();
	rl_redisplay();
}

void	set_signals(bool is_parent)
{
	if (is_parent)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
