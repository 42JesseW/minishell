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

/*
** FIXED - from name		(using $PATH)           -> {echo}
** - relative path	(from current workdir)  -> {../../bin/echo}
** 											|| {./echo}
**											|| {bin/echo}
** FIXED - absolute path	(starting at /)         -> {/bin/echo}
*/

#include <minishell.h>

/*
** DESCRIPTION
**	- Finds the path to the folder from where the given cmd can be executed
*/

char	*get_full_path(char *cmd, t_exe *exe)
{
	char	*path;
	int		len;

	if (ft_strncmp(cmd, "/", 1) == 0)
		return (cmd);
	len = ft_lstsize(exe->paths);
	while (len > 0)
	{
		path = ft_strjoin(exe->paths->content, cmd);
		if (path == NULL)
		{
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
				strerror(errno));
			return (NULL);
		}
		if (access(path, F_OK) != -1)
			return (path);
		exe->paths = exe->paths->next;
		len--;
	}
	ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, path, strerror(errno));
	return (NULL);
}
