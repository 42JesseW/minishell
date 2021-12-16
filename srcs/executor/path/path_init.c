/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe_paths.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 17:02:38 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/14 17:02:38 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** DESCRIPTION
**	- Splits the string of paths and adds each path to
**    the list of paths of the exe struct
** JOBS
** 1. Splits the string of paths on ':'
** 2. Adds a '/' to every path
** 3. Adds each path to the back of the list of paths of the exe struct
*/

int	store_paths(const char *str_paths, t_exe *exe) // TODO: ft_lstadd_back aanpassen (new eruit halen)
{
	int		idx;
	char	**split_paths;
	char	*current_path;

	split_paths = ft_strsplit(str_paths, ':');
	if (!split_paths)
		return (SYS_ERROR);
	idx = 0;
	while (split_paths[idx] != NULL)
	{
		current_path = ft_strjoin(split_paths[idx], "/");
		if (!current_path)
		{
			ft_strarrfree(&split_paths);
			return (SYS_ERROR);
		}
		ft_lstadd_back(&exe->paths, ft_lstnew(current_path));
		idx++;
	}
	ft_strarrfree(&split_paths);
	return (SUCCESS);
}

/*
** DESCRIPTION
**	- Takes the environmental variables from the shell struct,
**    searches for the PATH values and sends these to function store_paths
** JOBS
** 1. Takes the environmental variables from the shell struct
** 2. Searches for the PATH values
** 3. PATH values are send to the store_paths function
*/

int	init_paths(t_exe *exe, t_shell *shell)
{
	t_pair	*pair;

	while (shell->environ)
	{
		pair = shell->environ->content;
		if (ft_strncmp(pair->key, "PATH", 4) == 0)
		{
			if (store_paths(pair->val, exe) == SYS_ERROR)
			{
				dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, "Malloc",
					strerror(errno));
				return (SYS_ERROR);
			}
			return (SUCCESS);
		}
		else
			shell->environ = shell->environ->next;
	}
	return (SYS_ERROR);
}