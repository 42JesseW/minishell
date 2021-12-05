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

char	*get_full_path(char *cmd, t_exe *exe)
{
	char	*path;

	while (exe->paths)
	{
		path = ft_strjoin(exe->paths->content, cmd);
		if (path == NULL)
		{
			printf("Error - Malloc failed");
			return (NULL);
		}
		if (access(path, (F_OK & X_OK)) != -1)
			return (path);
		exe->paths = exe->paths->next;
	}
	return (NULL);
}
