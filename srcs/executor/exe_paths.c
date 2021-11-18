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

// Nadenken over generieke foutafhandeling: printf errors vervangen

#include <exe.h>

void	store_paths(const char *str_paths, t_exe *exe, t_shell *shell)
{
	int		idx;
	char	**split_paths;
	char	*current_path;

	split_paths = ft_strsplit(str_paths, ':');
	if (!split_paths)
		printf("Error - Malloc failed");
	idx = 0;
	while (split_paths[idx] != NULL)
	{
		current_path = ft_strjoin(split_paths[idx], "/");
		if (!current_path)
		{
			ft_strarrfree(&split_paths);
			printf("Error - Malloc failed");
		}
		ft_lstadd_back(&exe->paths, ft_lstnew(current_path));
		idx++;
	}
	ft_strarrfree(&split_paths);
}

int	init_paths(t_exe *exe, t_shell *shell)
{
	t_pair	*pair;

	while (shell->environ)
	{
		pair = shell->environ->content;
		if (ft_strncmp(pair->key, "PATH", 4) == 0)
		{
			store_paths(pair->val, exe, shell);
			return (0);
		}
		else
			shell->environ = shell->environ->next;
	}
	return (SYS_ERROR);
}
