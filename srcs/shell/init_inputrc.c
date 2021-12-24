/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_inputrc.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/24 10:29:08 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/24 10:29:08 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static const char	*get_inputrc_file_path(void)
{
	const char	*file_path;
	const char	*home;

	home = getenv("HOME");
	if (!home)
	{
		ft_dprintf(STDERR_FILENO, "get_inputrc_file_path: HOME not set");
		return (NULL);
	}
	file_path = ft_strjoin(home, "/.inputrc");
	return (file_path);
}

static int	create_inputrc_file(const char *file_path)
{
	char	*rc_data;
	int		fd;

	fd = open(file_path, O_CREAT | O_WRONLY, 0644);
	if (fd == SYS_ERROR)
		return (SYS_ERROR);
	rc_data = ft_strdup("$include /etc/inputrc\n"
						"set echo-control-characters Off\n\n");
	if (!rc_data || write(fd, rc_data, ft_strlen(rc_data)) < 0)
		return (SYS_ERROR);
	free(rc_data);
	close(fd);
	return (SUCCESS);
}

static int	option_exists(int fd)
{
	char	*line;
	int		ret;

	ret = 1;
	while (ret > 0)
	{
		ret = get_next_line(fd, &line);
		if (ret == SYS_ERROR)
			return (SYS_ERROR);
		if (line && ft_strcmp(line, INPUTRC_OPTION) == 0)
		{
			free(line);
			return (true);
		}
		free(line);
	}
	return (false);
}

static int	append_option(const char *file_path)
{
	int	ret;
	int	fd;

	fd = open(file_path, O_RDWR | O_APPEND);
	if (fd == SYS_ERROR)
		return (SYS_ERROR);
	ret = option_exists(fd);
	if (ret == SYS_ERROR || ret == true)
		return (ret);
	if (write(fd, INPUTRC_OPTION, ft_strlen(INPUTRC_OPTION)) < 0)
		return (SYS_ERROR);
	close(fd);
	return (SUCCESS);
}

int	init_inputrc(void)
{
	const char	*file_path;

	file_path = get_inputrc_file_path();
	if (!file_path)
		return (SYS_ERROR);
	if (access(file_path, F_OK) < 0)
	{
		if (create_inputrc_file(file_path) == SYS_ERROR)
			return (SYS_ERROR);
	}
	else
	{
		if (access(file_path, R_OK | W_OK) < 0)
		{
			ft_dprintf(STDERR_FILENO, "init_inputrc: invalid rights");
			return (SYS_ERROR);
		}
		if (append_option(file_path) == SYS_ERROR)
			return (SYS_ERROR);
	}
	return (SUCCESS);
}
