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
** - from name		(using $PATH)           -> {echo}
** - relative path	(from current workdir)  -> {../../bin/echo}
** 											|| {./echo}
**											|| {bin/echo}
** - absolute path	(starting at /)         -> {/bin/echo}
*/

#include <minishell.h>

/*
** DESCRIPTION
**	- Finds the path to the folder from where the given cmd can be executed
*/

char	*get_full_path(char *cmd, t_exe *exe)
{
	char	*path;

	while (exe->paths)
	{
		path = ft_strjoin(exe->paths->content, cmd);
		if (path == NULL)
		{
			dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
				strerror(errno));
			return (NULL);
		}
		if (access(path, F_OK) != -1)
			return (path);
		exe->paths = exe->paths->next;
	}
	dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, path, strerror(errno));
	return (NULL);
}
