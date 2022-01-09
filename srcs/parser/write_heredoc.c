/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   write_heredoc.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/09 13:32:58 by jessevander...#+#    #+#                 */
/*   Updated: 2022/01/09 13:32:58 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	write_heredoc_line(t_shell *shell, int fd, char *line)
{
	char	*resolved_line;

	resolved_line = resolve_dollar_heredoc(shell, line);
	if (!resolved_line)
	{
		free(line);
		return (SYS_ERROR);
	}
	write(fd, resolved_line, ft_strlen(resolved_line));
	write(fd, "\n", 1);
	free(resolved_line);
	return (SUCCESS);
}

static int	destruct_readline_environment(char *file_path, int fd)
{
	rl_event_hook = NULL;
	if (rl_done && g_exit_code_sig)
	{
		unlink(file_path);
		close(fd);
		rl_done = 0;
		return (SYS_ERROR);
	}
	signal(SIGINT, sigint_handler);
	return (SUCCESS);
}

int	default_event(void)
{
	return (0);
}

static int	init_readline_environment(char *file_path, int *fd)
{
	int	file_descriptor;

	file_descriptor = open(file_path, O_WRONLY | O_APPEND);
	if (file_descriptor == -1)
		return (SYS_ERROR);
	*fd = file_descriptor;
	rl_event_hook = default_event;
	signal(SIGINT, sigint_handler_heredoc);
	return (SUCCESS);
}

/*
** write_heredoc() reads from STDIN using readline
** and appends the data to HEREDOC_FILE using {fd}
** until the line returned by readline matches the
** {delimiter} string.
*/

int	write_heredoc(t_shell *shell, char *file_path, char *delimiter)
{
	char	*line;
	int		ret;
	int		fd;

	if (init_readline_environment(file_path, &fd) == SYS_ERROR)
		return (SYS_ERROR);
	line = readline(HEREDOC_PROMPT);
	while (line)
	{
		if (g_exit_code_sig || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ret = write_heredoc_line(shell, fd, line);
		free(line);
		if (ret == SYS_ERROR)
		{
			unlink(file_path);
			close(fd);
			return (SYS_ERROR);
		}
		line = readline(HEREDOC_PROMPT);
	}
	return (destruct_readline_environment(file_path, fd));
}
