/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_full_path.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/27 15:03:19 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/27 15:03:19 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Finds the path to the folder from where the given cmd can be executed
*/

char	*get_path_environ(char *cmd, t_exe *exe)
{
	char	*path;
	t_list	*path_node;
	int		len;

	path_node = exe->paths;
	len = ft_lstsize(path_node);
	while (len > 0)
	{
		path = ft_strjoin(path_node->content, cmd);
		if (path == NULL)
			return (NULL);
		if (access(path, F_OK) != -1)
			return (path);
		free(path);
		path_node = path_node->next;
		len--;
	}
	return (cmd);
}

char	*get_path_relative(char *cmd)
{
	char	buff[256];
	char	*path;

	if (getcwd(buff, sizeof(buff)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "getcwd",
			strerror(errno));
		return (NULL);
	}
	else
	{
		if (ft_strncmp(cmd, "./", 2) == 0)
		{
			path = ft_strjoin(buff, (cmd + 1));
			return (path);
		}
		else
		{
			path = ft_strnjoin(3, buff, "/", cmd);
			return (path);
		}
	}
}

char	*get_full_path(char *cmd, t_exe *exe)
{
	char	*path;

	path = NULL;
	if (ft_strnstr(cmd, "/", ft_strlen(cmd)) != NULL)
	{
		if (ft_strncmp(cmd, "/", 1) == 0)
			path = cmd;
		else
			path = get_path_relative(cmd);
		if (access(path, F_OK) != -1)
			return (path);
		else
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, path,
				strerror(errno));
		return (NULL);
	}
	else
		path = get_path_environ(cmd, exe);
	return (path);
}
